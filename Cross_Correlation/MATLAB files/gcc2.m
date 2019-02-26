clc
clear
format compact
[d, Fs] = audioread('6forward_rotate.wav');

add = 5;
division = 512;
%to work on a second of samples every iteration
seconds = floor(length(d)/division);
aves = floor(seconds/add);

%history of drone movement - leds turned on
mics = zeros(aves,1);
aveofmics = zeros(add, 1);
p = 1;

        
N = 512;
M = 8;

Ncorr = 2*N-1;
NFFT = 2^nextpow2(Ncorr);
lags = (-(Ncorr-1)/2:(Ncorr-1)/2);
lags = lags/Fs;

qw= 1;
c = zeros(NFFT,8);
for j=1:seconds 
    for i=1:8    
        e(:,i) = d(division*(j-1)+1:division*j,i);
    end

    %switch leds 1 and 2 - recording mistake
    e(:,1) = d(division*(j-1)+1:division*j,2);
    e(:,2) = d(division*(j-1)+1:division*j,1);
            
    for fftaddup =1:8
        c(:,fftaddup) = c(:,fftaddup) + fft(e(:,fftaddup), NFFT);
        c(:,fftaddup) = c(:,fftaddup) ./ 2;
    end

    if(qw == 10)
        for i=1:M     
            R12 = c(:,i) .* conj(c(:,3));
            X2 = zeros(length(R12),1);
    
            for k = 2:length(R12)
                X2(k) = R12(k)/abs(R12(k));
            end
    
            r12_temp = fftshift(ifft(X2));
            r12(:,i) = r12_temp(NFFT/2+1 - (Ncorr-1)/2 : NFFT/2+1 + (Ncorr-1)/2);
 
            [val,idx] = max(abs(r12(:,i)));
            tau(i) = lags(idx);
        end

%mic 4 broken
        tau(4) = 10000;

        [~,led] = min(tau);
        mics(p) = led;
        p = p + 1;
        qw = 0;
        c = zeros(NFFT,8);
    end
    
    qw = qw + 1;
end

plot(mics);
ylim([1 8]);

xlabel('Time');
ylabel('Led to turn on');
title('Counter clock wise rotation');