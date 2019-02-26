#include <stdio.h>
#include <math.h>
#include "arm_math.h"
#include "math_helper.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_sai.h"

#define SAI I2S0
#define SAI_CLKSRC kCLOCK_CoreSysClk
#define SAI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define SAI_IRQ I2S0_Rx_IRQn
#define SAI_TxIRQHandler I2S0_Tx_IRQHandler

_Bool isFinished = 0;

void SAI_UserCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
	//printf("in handle \n");
    if (kStatus_SAI_RxIdle == status)
    {
    	printf("finished\n");
        isFinished = true;
    }
}

int32_t main(void)
{
	BOARD_InitBootPins();
	BOARD_BootClockRUN();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();

	uint16_t samples = 16;
	uint16_t Ncorr = 2*samples - 1;
	uint8_t achannels = 5;
	uint16_t NFFT = pow(2, ceil(log(Ncorr)/log(2)));
	printf("NFFT: %d\n\n", NFFT);

	float32_t aves[achannels];
	float32_t haves[3];
	float32_t az = 0;

	float32_t taus[achannels];

	float32_t Ak[NFFT];
	float32_t Ar[NFFT];

	struct channels {
		float32_t Bk[NFFT];
		float32_t Br[NFFT];
	};

	struct channels data[achannels-1];

	float32_t AxB[NFFT];
	float32_t result[NFFT];
	float32_t max_lag = 0;
	uint32_t lag_ind = 0;
	int16_t lags[Ncorr];
	uint16_t p;
	uint16_t yy = (Ncorr-1)/2;

	sai_config_t config;
	sai_transfer_format_t format;
	sai_handle_t g_handle;
	sai_transfer_t rxfer;
	uint32_t mclkSourceClockHz = 0U;
	int32_t myd[samples*8];
	int32_t *temp32;
	float32_t temp32data;

    config.protocol = kSAI_BusLeftJustified ;
	config.syncMode = kSAI_ModeAsync;
	config.mclkSource = kSAI_MclkSourceSysclk;
	config.bclkSource = kSAI_BclkSourceMclkDiv;
	config.masterSlave = kSAI_Master;
	config.mclkOutputEnable = true;

	SAI_RxInit(SAI, &config);

	rxfer.data = (uint8_t*)myd;
	rxfer.dataSize = sizeof(myd)/sizeof(uint32_t)*4;

	format.bitWidth = 32U;
	format.sampleRate_Hz = 48000U;
	format.stereo = kSAI_MonoLeft;
	format.masterClockHz = 256U * format.sampleRate_Hz;
	format.channel = 0;
	format.protocol = config.protocol;
	format.watermark = 8;
	format.isFrameSyncCompact = 0;

	mclkSourceClockHz = SAI_CLK_FREQ;

	SAI_TransferRxCreateHandle(SAI, &g_handle, SAI_UserCallback, rxfer.data);
	SAI_TransferRxSetFormat(SAI, &g_handle, &format, mclkSourceClockHz, format.masterClockHz);
	for(uint16_t i = 0; i < Ncorr; i++)
			lags[i] = i - yy;

	arm_rfft_fast_instance_f32 rfft_instance;
	arm_rfft_fast_instance_f32 *rfft_instance_ptr = (arm_rfft_fast_instance_f32*) &rfft_instance;

  	arm_rfft_fast_init_f32(rfft_instance_ptr, NFFT);

  	for(int times = 0; times < 1; times++)//while(1)
  	{
  		arm_fill_f32(0.0,  taus, 8);
  		arm_fill_f32(0.0,  aves, achannels);
  		arm_fill_f32(0.0,  haves, 3);

  		for(int8_t k = 0; k < 8; k++)
  		{
  			arm_fill_f32(0.0,  Ak, NFFT);

  			for(int ip = 0; ip < achannels-1; ip++)
  				arm_fill_f32(0.0,  data[ip].Bk, NFFT);

  			SAI_TransferReceiveNonBlocking(SAI, &g_handle, &rxfer);
  			isFinished = 0;

  			while (isFinished != true)
  			{
  				//printf("waiting\n");
  			}

  			//SAI_TransferAbortReceive(SAI, &g_handle);

  			temp32 = (int32_t *)myd;

  			for(int ix = 0; ix < samples; ix++)
  			{
  				temp32data = (int32_t) *temp32;
  				Ak[ix] = (float32_t) temp32data;
  				temp32++;

  				for(int ip = 0; ip < achannels-1; ip++)
  				{
  					temp32data = (int32_t) *temp32;
  					data[ip].Bk[ix] = (float32_t) temp32data;
  					temp32++;
  				}
  			}

  			arm_rfft_fast_f32(rfft_instance_ptr, Ak, Ar, 0);
  			arm_cmplx_conj_f32(Ar, Ar, samples);

  			taus[0] = 0;
  			//printf("tau[%d]: %f\n", taus[0]);

  			for(int b = 0; b < achannels-1; b++)
  			{
  				arm_rfft_fast_f32(rfft_instance_ptr, data[b].Bk, data[b].Br, 0);

  				arm_cmplx_mult_cmplx_f32(Ar, data[b].Br, AxB, samples);
  				arm_cmplx_mag_f32(AxB, data[b].Bk, samples);


  				for(uint16_t i = 0; i < samples; i++)
  				{
  					p = 2*i;
  					AxB[p] = AxB[p]/data[b].Bk[i];
  					AxB[p + 1] = AxB[p + 1]/data[b].Bk[i];
  				}

  				AxB[0] = 0;
  				AxB[1] = 0;

  				arm_rfft_fast_f32(rfft_instance_ptr, AxB, result, 1);

  				arm_copy_f32(result,  AxB, samples);
  				arm_copy_f32(result + samples, result, samples);
  				arm_copy_f32(AxB, result + samples, samples);

  				arm_abs_f32(result, result, NFFT);
  				result[0] = 0;
  				result[Ncorr-1] = 0;

  				arm_max_f32(result, NFFT, &max_lag, &lag_ind);

  				taus[b+1] = lags[lag_ind-1]/44100.0;
  				printf("tau[%d]: %f\n", b+1, taus[b+1]);
  			}

  			arm_min_f32(taus, 8, &max_lag, &lag_ind);
  			printf("mic: %d\n", lag_ind);
  			aves[lag_ind] += 1;

  			if(lag_ind < 4)
  				az = fabs(taus[lag_ind + 4] - taus[lag_ind]);

  			else if(lag_ind < 8)
  				az = fabs(taus[lag_ind - 4] - taus[lag_ind]);

  			if(az <= 0.000195)
  				haves[2] += 1;

  			else if(az <= 0.000389)
  				haves[1] += 1;

  			else if(az <= 0.000672)
  				haves[0] += 1;

  		}

  		arm_max_f32(aves, achannels, &max_lag, &lag_ind);
  		printf("direction: %d\n", lag_ind);

  		arm_max_f32(haves, 3, &max_lag, &lag_ind);
  		printf("azimuth: %d\n", lag_ind);
	}

  	return 0;
}
