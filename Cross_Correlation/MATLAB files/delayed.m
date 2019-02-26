clear 
clc

[x, Fs] = audioread('l.wav');
[y, Fs1] = audioread('r.wav');

X = fft(x);
XP2 = abs(X/length(x));
XP1 = XP2(1:length(x)/2+1);
XP1(2:end-1) = 2*XP1(2:end-1);
xf = Fs*(0:(length(x)/2))/length(x);
figure;
plot(xf,XP1) 
title('Single-Sided Amplitude Spectrum of S(t)')
xlabel('x (Hz)')
ylabel('|xP1(f)|')

Y = fft(y);
YP2 = abs(Y/length(y));
YP1 = YP2(1:length(y)/2+1);
YP1(2:end-1) = 2*YP1(2:end-1);
yf = Fs1*(0:(length(y)/2))/length(y);
figure;
plot(yf,YP1) 
title('Single-Sided Amplitude Spectrum of S(t)')
xlabel('y (Hz)')
ylabel('|yP1(f)|')

[~,xm] = max(abs(XP1));
[~,ym] = max(abs(YP1));

X2 = zeros(length(XP1),1);
Y2 = zeros(length(YP1),1);

%X2 = XP1./abs(XP1);
%Y2 = YP1./abs(YP1);
p = 1;
X2(xm-p:xm+p) = XP1(xm-p:xm+p);
Y2(ym-p:ym+p) = YP1(ym-p:ym+p);
figure;
plot(yf,Y2);
v = X2.*conj(Y2);
v = v./abs(v);
v(1) = 0;

V2 = fftshift(ifft(v));
maxv2 = max(abs(V2))
%{
V1 = ifft(X .* conj(Y));
figure;
plot(V1);
%}


%{
xr = ifft(X2, 'symmetric');
yr = ifft(Y2, 'symmetric');

[N,M] = size(xr);

Ncorr = 2*N-1;
NFFT = 2^nextpow2(Ncorr);
lags = (-(Ncorr-1)/2:(Ncorr-1)/2);
%lags = (1:Ncorr);
lags = lags/Fs;
r12 = mygccphat(xr, yr, NFFT, N);
figure
plot(r12);
figure;
plot(xr);
title('X after fft');

figure;
plot(yr);
title('Y after fft');

xc = xr(1:8937);
yc = yr(1:8937);

[acor,lag] = xcorr(xc,yc);
[~,I] = max(abs(acor));
lagDiff = lag(I)

timeDiff = lagDiff/Fs

figure
plot(lag,acor)
a3 = gca;
a3.XTick = sort([-30000:1000:30000 lagDiff]);
%}
function r12 = mygccphat(x, xref, NFFT, N)
    R12 = fft(x,NFFT) .* conj(fft(xref,NFFT));
    r12_temp = fftshift(ifft(R12));
    r12 = r12_temp(NFFT/2+1-(N-1)/2:NFFT/2+1+(N-1)/2,:);
%r12 = r12_temp(1:NFFT/2+1+(N-1)/2,:);
end