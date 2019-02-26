clear
A = [-0.808920   1.357369   1.180861  -0.504544   1.762637  -0.703285 1.696966   0.620571  -0.151093  -0.100235  -0.872382  -0.403579 -0.860749  -0.382648  -1.052338   0.128113  -0.646269   1.093377  -2.209198   0.471706   0.408901   1.266242   0.598252   1.176827  -0.203421   0.213596  -0.851964  -0.466958   0.021841  -0.698938  -0.604107   0.461778  -0.318219   0.942520   0.577585   0.417619   0.614665   0.563679  -1.295073  -0.764437   0.952194  -0.859222  -0.618554  -2.268542  -1.210592   1.655853  -2.627219  -0.994249  -1.374704   0.343799   0.025619   1.227481  -0.708031   0.069355  -1.845228  -1.570886   1.010668  -1.802084   1.630088   1.286090  -0.161050  -0.940794   0.367961   0.291907];

x = A';
Ncorr = 2*length(x)-1;
NFFT = 2^nextpow2(Ncorr);

for n = length(x)+1:NFFT
    x(n) = 0;
end

for k =1:NFFT
    X(k) = 0;
    for n=1:NFFT
       X(k) = X(k) + x(n)*exp(-1i*2*pi*(k-1)*(n-1)/NFFT);
        %X(n) = exp(-1i*2*3.14159*(k-1)*(n-1)/NFFT);
    end
end




