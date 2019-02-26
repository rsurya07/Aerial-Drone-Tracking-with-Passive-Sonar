#include <stdio.h>
#include <complex.h>
#include <math.h>
#include "arm_math.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

# define M_PI		3.14159265358979323846
#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

void separate(complex float * DATA, unsigned short N);
void fft2(complex float * DATA, unsigned short N, short ifft_flag);
void fftshift(complex float * DATA, unsigned short N);
double complex cpow(double complex a, double complex z);
float cab(complex float a);

extern float testInputA_f32[882];
extern float testInputB_f32[882];
void delay(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 800000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}
int main()
{
	gpio_pin_config_t led_config = {
	        kGPIO_DigitalOutput, 0,
	    };

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    GPIO_PinInit(BOARD_my_led_GPIO, BOARD_my_led_GPIO_PIN, &led_config);

    unsigned short samples = 882;
    unsigned short Ncorr = 2*samples - 1;
    unsigned short NFF = pow(2, ceil(log(Ncorr)/log(2)));
    float NFFT = NFF/1.0;
    float max_lag;
    unsigned short lag_ind[2];
    short fft = -1;
    short ifft = 1;
    float * inpA;
    float * inpB;



    inpA = &testInputA_f32[0];
    inpB = &testInputB_f32[0];

    complex float A[NFF];
    complex float B[NFF];

    //float fft_shifted[NFF];
    float final[Ncorr];
    float lags[Ncorr];
    //float power;

    unsigned short i;


    //*******READ FROM TEXT - DELETE LATER******
    //FILE * fd = fopen("data.txt", "r");

    //printf("Reading data\n");

    for(i = 0; i < samples; i++)
    {
    	//fscanf(fd, "%f", &power);
    	A[i] = inpA[i];
    }

    for(i = 0; i < samples; i++)
    {
        //fscanf(fd, "%f", &power);
        B[i] = inpB[i];
    }
    //fclose(fd);
    //********************************************/

    for(i = samples; i < NFF; i++)
    {
    	A[i] = 0;
    	B[i] = 0;
    }

    for(i = 0; i < Ncorr; i++)
        		lags[i] = i -(Ncorr-1)/2;

   // printf("Converting to frequency domain\n");
    fft2(A, NFF, fft);
    fft2(B, NFF, fft);

   // for(i = 0; i < NFF; i++)
    //	printf("[%3d] =  %10f + %10f i\n", i+1, creal(A[i]), cimag(A[i]));

   // printf("\n\n");

  //  for(i = 0; i < NFFT; i++)
    //	printf("[%3d] =  %10f + %10f i\n", i+1, creal(B[i]), cimag(B[i]));

  //  printf("\n\nConjugate\n\n");
    for(i = 0; i < NFFT; i++)
    {
    	A[i] = conj(A[i]);
    	//printf("[%3d] =  %10f + %10f i\n", i+1, creal(A[i]), cimag(A[i]));
    }

    //printf("\n\nB times conj of A\n\n");

    for(i = 0; i < NFFT; i++)
    {
    	B[i] = (creal(B[i])*creal(A[i]) - cimag(B[i])*cimag(A[i])) + I*(creal(B[i])*cimag(A[i]) + creal(A[i])*cimag(B[i]));
    	B[i] = B[i]/cab(B[i]);
    	//printf("[%3d] =  %10f + %10f i\n", i+1, creal(B[i]), cimag(B[i]));
    }

    B[0] = 0;

    fft2(B, NFF, ifft);

    //printf("\n\nIFFT\n\n");
    for(i = 0; i < NFF; i++)
    {
    	B[i] = B[i]/NFFT;
    	//printf("[%3d] =  %10f + %10f i\n", i+1, creal(B[i]), cimag(B[i]));
    }

    fftshift(B, NFF);

   // printf("\n\nFFT shifted\n\n");
    //for(i = 0; i < NFF; i++)
    //	printf("[%3d] =  %10f\n", i+1, creal(B[i]));

    for(i = 0; i < Ncorr; i++)
    	final[i] = creal(B[NFF/2 - (Ncorr-1)/2 + i]);

    max_lag = fabs(final[0]);
    lag_ind[0] = 0;

   // printf("\n\nFinal\n\n");
    for(i = 0; i < Ncorr; i++)
    {
    	//printf("[%3d] =  %10f\n", i+1, final[i]);

    	if(fabs(final[i]) > max_lag)
    	{
    		max_lag = fabs(final[i]);
    		lag_ind[0] = i;
    	}
    }

    printf("\n\nlag = %.10f\n", lags[lag_ind[0]]/44100);

    while (1)
        {
            delay();
            GPIO_PortToggle(BOARD_my_led_GPIO, 1u << BOARD_my_led_GPIO_PIN);
        }

    return 0 ;
}

void fft2(complex float * DATA, unsigned short N, short ifft_flag)
{
	if(N > 1)
	{
		separate(DATA, N);
		fft2(DATA, N/2, ifft_flag);
		fft2(DATA + N/2, N/2, ifft_flag);

		complex float e = 0;
		complex float o = 0;
		complex float w = 0;

		for(unsigned short k = 0; k < N/2; k++)
		{
			e = DATA[k];
			o = DATA[k + N/2];
			w = cpow(exp(1), (ifft_flag * I * 2.0 * M_PI * k / N));

			DATA[k] = e + ((creal(o)*creal(w) - cimag(o)*cimag(w)) + I*(creal(o)*cimag(w) + creal(w)*cimag(o)));
			DATA[k + N/2] = e - ((creal(o)*creal(w) - cimag(o)*cimag(w)) + I*(creal(o)*cimag(w) + creal(w)*cimag(o)));
		}
	}
}

void separate(complex float * DATA, unsigned short N)
{
	complex float temp[N/2];
	unsigned short p = 0;

	for(p = 0; p < N/2; p++)
		temp[p] = DATA[p * 2 + 1];

	for(p = 0; p < N/2; p++)
		DATA[p] = DATA[p*2];

	for(p = 0; p < N/2; p++)
		DATA[p + N/2] = temp[p];
}

void fftshift(complex float * DATA, unsigned short N)
{
	complex float temp[N/2];
	unsigned short p = 0;

	for(p = 0; p < N/2; p++)
	{
		temp[p] = DATA[p];
		DATA[p] = DATA[N/2 + p];
	}

	for(p = 0; p < N/2; p++)
		DATA[N/2 + p] = temp[p];
}

double complex cpow(double complex a, double complex z)
{
	double complex w;
	double x, y, r, theta, absa, arga;

	x = creal(z);
	y = cimag(z);
	absa = a;//cabs(a);
	if (absa == 0.0) {
		return (0.0 + 0.0 * I);
	}
	arga = carg(a);
	r = pow(absa, x);
	theta = x * arga;
	if (y != 0.0) {
		r = r * exp(-y * arga);
		theta = theta + y * log(absa);
	}
	w = r * cos(theta) + (r * sin(theta)) * I;
	return w;
}

float cab(complex float a)
{
	a = pow(creal(a), 2) + pow(cimag(a), 2);
	return sqrt(a);

}
