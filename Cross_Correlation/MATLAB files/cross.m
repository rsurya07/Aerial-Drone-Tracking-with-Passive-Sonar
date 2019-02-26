clear 
clc

[d, Fs] = audioread('synth_data_1.wav');
%[x, Fs] = audioread('l2.wav');
%[y, Fs1] = audioread('r2.wav');
x = d(:,1);
y = delayseq(x,0.5e-3,Fs);
Fs1 = Fs;
t1 = (0:length(x)-1)/Fs;
t2 = (0:length(y)-1)/Fs1;


subplot(2,1,1)
plot(t1,x)
title('l')

subplot(2,1,2)
plot(t2,y)
title('r')
xlabel('Time (s)')

[acor,lag] = xcorr(y,x);
max(abs(acor))
[~,I] = max(abs(acor));
lagDiff = lag(I)

timeDiff = lagDiff/Fs

figure
plot(lag,acor)
a3 = gca;
a3.XTick = sort([-30000:1000:30000 lagDiff]);


s1al = x(-lagDiff+1:end);
t1al = (0:length(s1al)-1)/Fs;

subplot(2,1,1)
plot(t1al,s1al)
title('l, aligned')

subplot(2,1,2)
plot(t2,y)
title('y')
xlabel('Time (s)')

c = gccphat(x, y)/Fs;
