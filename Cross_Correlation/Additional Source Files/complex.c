#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

int Ncorr; 
int NFF; 
double NFFT; 
int fft = -1;
int ifft = 1;

  	
void separate (complex double* a, int n) 
{
    complex double * b = (complex double*) malloc(sizeof(complex double) * n/2);  
    
    for(int i=0; i<n/2; i++)    // copy all odd elements to heap storage
        b[i] = a[i*2+1];
        
    for(int i=0; i<n/2; i++)    // copy all even elements to lower-half of a[]
        a[i] = a[i*2];
        
    for(int i=0; i<n/2; i++)    // copy all odd (from heap) to upper-half of a[]
        a[i+n/2] = b[i];
        
    free(b);                 
}

void fft2 (complex double * X, int N, int ifft_flag) 
{
    if(N < 2) 
    {
        // bottom of recursion.
        // Do nothing here, because already X[0] = x[0]
    } 
    
    else 
    {
        separate(X,N);      // all evens to lower half, all odds to upper half
        fft2(X, N/2, ifft_flag);   // recurse even items
        fft2(X+N/2, N/2, ifft_flag);   // recurse odd  items
        // combine results of two half recursions
        
        for(int k=0; k<N/2; k++) 
        {
            complex double e = X[k];   // even
            complex double o = X[k+N/2];   // odd
                         // w is the "twiddle-factor"
           complex double w = cpow(exp(1), (ifft_flag*I*2. * M_PI * k / N));
           // w = cpow(w,I);
            
            X[k] = e + w * o;
            X[k+N/2] = e - w * o;
           // printf("[%3d] =  %10f + %10f i\n", k+1, creal(X[k]), cimag(X[k]));
        }
    }
}

void fftshift(double * shifted, complex double * data, int N)
{
	double * temp = (double *) malloc(sizeof(double) * N/2);
	
	for(int i = 0; i < N/2; i++)
	{
		temp[i] = creal(data[i]);
		shifted[i] = creal(data[N/2 + i]);
	} 
	
	for(int i = 0; i < N/2; i++)
		shifted[N/2 + i] = temp[i];
	
	free(temp);
}
	
int main()
{
	int samples = 64;
	
	Ncorr = 2*samples - 1;
	NFF = pow(2, ceil(log(Ncorr)/log(2)));
	//NFF = samples;
	NFFT = NFF/1.0;

	printf("NFFT = %f\n", NFFT);
	
	double max_lag;
	int lag_ind;
	
	complex double A[NFF];	
	complex double B[NFF];
	complex double A_x_conjB[NFF];
	double lags[Ncorr];
	
	for(int i = 0; i < Ncorr; i++)
	{
		lags[i] = i -(Ncorr-1)/2;
		printf("[%d] %f\n", i, lags[i]);
	}
	
	double power;
	double fft_shifted[NFF];
	double final[Ncorr];
	/*
	FILE * fd = fopen("2ch_mus_lab.txt", "r");
	
	printf("Reading data\n");
	
	for(int i = 0; i < Fs; i++)
	{
		fscanf(fd, "%lf", &power);
		printf("%f ", power);
		A[i] = power;
		fscanf(fd, "%lf", &power);
		printf(" %f \n", power);
		B[i] = power;
	}
	*/
	
	FILE * fd = fopen("data.txt", "r");
	
	printf("Reading data\n");
	
	for(int i = 0; i < samples; i++)
	{
		fscanf(fd, "%lf", &power);
		A[i] = power;
	}
	
	printf("\n\nA read\n\n");
	
	for(int i = 0; i < samples; i++)
	{
		fscanf(fd, "%lf", &power);		
		B[i] = power;
	}
	
	printf("\n\nData read\n");
	
	for(int i = samples; i < NFF; i++)
	{
		A[i] = 0;
		B[i] = 0;
	}  
		


	printf("Converting to frequency domain\n");
  	fft2(A, NFF, fft);
  	fft2(B, NFF, fft);
  	
  	
  
  	
  	for(int k = 0; k < NFF; k++)
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(A[k]), cimag(A[k]));
  		
  	
  	printf("\n\n\n");
  	
  	for(int k = 0; k < NFFT; k++)
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(B[k]), cimag(B[k]));
  	
  	  		
  	
  	printf("\n\nConjugate\n\n");
  	
  	for(int k = 0; k < NFFT; k++)
  	{
  		A[k] = conj(A[k]);
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(A[k]), cimag(A[k]));
  	}
    
    
    printf("\n\nB times conj of A\n\n");
  	
  	for(int k = 0; k < NFFT; k++)
  	{
  		A_x_conjB[k] = A[k] * B[k];
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(A_x_conjB[k]), cimag(A_x_conjB[k]));
  	}
  	
  	printf("\n\nDivide by abs\n\n");
  	
  	for(int k = 0; k < NFFT; k++)
  	{
  		A_x_conjB[k] = A_x_conjB[k]/cabs(A_x_conjB[k]);
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(A_x_conjB[k]), cimag(A_x_conjB[k]));
  	}
  	
  	A_x_conjB[0] = 0;
	A_x_conjB[NFF/2] = 0;
  
  	
  	fft2(A_x_conjB, NFF, ifft);
  	
  	
  	printf("\n\nIFFT\n\n");
  	
  	for(int k = 0; k < NFF; k++)
  	{
  		A_x_conjB[k] = A_x_conjB[k]/NFFT;
  		//printf("[%3d] =  %10f\n", k+1, creal(A2[k]);
  		printf("[%3d] =  %10f + %10f i\n", k+1, creal(A_x_conjB[k]), cimag(A_x_conjB[k]));
  	}
  	
  	fftshift(fft_shifted, A_x_conjB, NFF);
  	
  	printf("\n\nFFT shifted\n\n");
  	
  	for(int k = 0; k < NFFT; k++)
  		printf("[%3d] =  %10f\n", k+1, fft_shifted[k]);
  		
  	for(int i = 0; i < Ncorr; i++)
  		final[i] = fft_shifted[NFF/2 - (Ncorr-1)/2 + i];
  		
  	max_lag = fabs(final[0]);
  	lag_ind = 0;
  	
  	printf("\n\nFinal\n\n");
  	for(int i = 0; i < Ncorr; i++)
  	{
  		printf("[%3d] =  %10f\n", i+1, final[i]);
  		
  		if(fabs(final[i]) > max_lag)
  		{
  			max_lag = fabs(final[i]);
  			lag_ind = i;
  		}
  	}
  	
  	printf("\n\nlag = %.10f    %f    %d\n", lags[lag_ind]/44100, max_lag, lag_ind);
  	
    
    return 0;
}
/*
void myfft(double complex fft_result[], double data[])
{
	double complex result;
	
	for(int k = 0; k < NFFT; k++)
  	{
  		fft_result[k] = 0;
  		
  		for(int n = 0; n < NFFT; n++)
  		{
  			result = cpow(exp(1), -I*2*M_PI*(k)*(n)/NFFT);
  			//result = cpow(power, -I);
  			fft_result[k] = fft_result[k] + data[n]* result;
  			//printf("[%d][%d] %f \n", k,n,power);//creal(result), cimag(result));
  		}
  			
    	
    }

}*/
