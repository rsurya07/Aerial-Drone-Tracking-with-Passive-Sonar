#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_sai.h"
#include "MK64F12.h"


#define SAI I2S0
#define SAI_CLKSRC kCLOCK_CoreSysClk
#define SAI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define SAI_IRQ I2S0_Rx_IRQn
#define SAI_TxIRQHandler I2S0_Tx_IRQHandler

_Bool isFinished = 0;

void SAI_UserCallback(I2S_Type *base, sai_handle_t *handle, status_t status, void *userData)
{
    if (kStatus_SAI_RxIdle == status)
    {
        isFinished = true;
    }
}

int main(void){

	BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    int samples = 64;
    sai_config_t config;
    sai_transfer_format_t format;
    sai_handle_t g_handle;
    sai_transfer_t rxfer;
    uint32_t mclkSourceClockHz = 0U;
    int32_t myd[samples];

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

    SAI_TransferReceiveNonBlocking(SAI, &g_handle, &rxfer);

      // EnableIRQ(SAI_IRQ);
       //SAI_RxEnableInterrupts(SAI, kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable);
       //SAI_RxEnable(SAI, true);

    while (isFinished != true)
    {

    }

    SAI_TransferAbortReceive(SAI, &g_handle);

    for(int i = 0; i < samples; i++)
    {
    	printf("%d %d\n", i, myd[i]);
    }
    printf("\ndone\n");

    return 0 ;
}
