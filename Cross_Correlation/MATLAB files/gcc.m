clc
clear
format compact

[d, Fs] = audioread('6forward_rotate.wav');

%d = zeros(length(e),8);
%{
[d(:,1), Fs] = audioread('synth_data_1.wav');
[d(:,2), Fs] = audioread('synth_data_2.wav');
[d(:,3), Fs] = audioread('synth_data_3.wav');
[d(:,4), Fs] = audioread('synth_data_4.wav');
[d(:,5), Fs] = audioread('synth_data_5.wav');
[d(:,6), Fs] = audioread('synth_data_6.wav');
[d(:,7), Fs] = audioread('synth_data_7.wav');
[d(:,8), Fs] = audioread('synth_data_8.wav');

%}

%{
[d(:,1),Fs] = audioread('in26_1.wav');
[d(:,2),Fs] = audioread('in26_2.wav');
[d(:,3),Fs] = audioread('in26_3.wav');
[d(:,4),Fs] = audioread('in26_4.wav');
[d(:,5),Fs] = audioread('in26_5.wav');
[d(:,6),Fs] = audioread('in26_6.wav');
[d(:,7),Fs] = audioread('in26_7.wav');
[d(:,8),Fs] = audioread('in26_8.wav');
%}
add = 10;
division = 512;
%to work on a second of samples every iteration
seconds = floor(length(d)/division);
aves = floor(seconds/add);

N = division;
M = 8;
Ncorr = 2*N-1;
NFFT = 2^nextpow2(Ncorr);
lags = (-(Ncorr-1)/2:(Ncorr-1)/2);
lags = lags/Fs;

%history of drone movement - leds turned on
mics = zeros(aves,1);
heights = zeros(aves, 1);
aveofmics = zeros(add, 1);
aveheight = zeros(add, 1);
p = 1;
qw= 1;

for j=1:seconds 
    for i=1:M    
        c(:,i) = d(division*(j-1)+1:division*j,i);
    end

    %switch leds 1 and 2 - recording mistake
    c(:,1) = d(division*(j-1)+1:division*j,2);
    c(:,2) = d(division*(j-1)+1:division*j,1); 
    
    for i=1:M  
        R12 = fft(c(:,i),NFFT) .* conj(fft(c(:,6),NFFT));    
        X2 = zeros(length(R12),1);
    
        for k = 2:length(R12)
            X2(k) = R12(k)/abs(R12(k));
        end
    
        r12_temp = fftshift(ifft(X2));
        r12(:,i) = r12_temp(NFFT/2+1 - (Ncorr-1)/2 : NFFT/2+1 + (Ncorr-1)/2);
 
        [val,idx] = max(abs(r12(:,i)));
        tau(i) = lags(idx);
        %fprintf('%0.10f  ',tau(i));
    end
    %fprintf('\n');

%mic 4 broken
    tau(4) = 10000;
    
    [~,led] = min(tau);
    %mics(j) = led;
    
    if(led < 5)
        az = abs(tau(led + 4) - tau(led));
        %gp = gccphat(c(:,led+4),c(:,led),Fs)
    else
        az = abs(tau(led - 4) - tau(led));
        %gp = gccphat(c(:,led-4),c(:,led),Fs)
    end
    
    if(az <=  0.000195)
    	aveheight(qw) = 3;
            
    elseif(az <= 0.000389)
    	aveheight(qw) = 2;
            
	%elseif(az <= 0.000672)
    else
    	aveheight(qw) = 1;
	end
  
    aveofmics(qw) = led;
    
   
if(qw == add)
        mics(p) = mode(aveofmics);
        heights(p) = mode(aveheight);
        p = p + 1;
        aveofmics = zeros(add, 1);
        aveheight = zeros(add ,1);
        qw = 0;
end

        qw = qw + 1; 
  
end

plot(mics);
ylim([1 8]);
xlabel('Time');
ylabel('Led to turn on');
title('Counter clock wise rotation');

figure
plot(heights)
ylim([0 5]);