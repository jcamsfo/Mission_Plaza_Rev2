

#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "ftd2xx.h"
#include <thread>


using namespace std;




void FTDI_Init(void)
{


    // FTDI hardware stuff


	DWORD dwError, dwPriClass;
	DWORD bytes_written;
	FT_HANDLE ftHandle;
	FT_STATUS ftStatus;
	UCHAR Mask = 0xff;
	UCHAR Mode;
	DWORD EventDWord;
	DWORD RxBytes;
	DWORD TxBytes;
	DWORD BytesWritten;
	DWORD BytesReceived;



	ftStatus = FT_Open(0, &ftHandle);
	printf("\n status only = %d   \n", ftStatus);

	if(ftStatus != FT_OK)
	{
		// FT_Open failed return;
		printf("FT_Open FAILED! status = %d    \r\n", ftStatus  );
		exit(0);
	}
	else
	{
		printf("\n made it to FTDI initialize status =  %d   \n", ftStatus);
		//set interface into FT245 Synchronous FIFO mode
		Mode = 0x00; //reset mode
		ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
		Mode = 0x40; //Sync FIFO mode
		ftStatus = FT_SetBitMode(ftHandle, Mask, Mode);
		if (ftStatus != FT_OK) printf("FT_SetBitMode FAILED! \r\n"); // FT_SetBitMode FAILED!
		FT_SetLatencyTimer(ftHandle, 2);
		FT_SetUSBParameters(ftHandle,0x10000, 0x10000);
		FT_SetFlowControl(ftHandle, FT_FLOW_RTS_CTS, 0x0, 0x0);
		FT_Purge(ftHandle, FT_PURGE_RX);
		printf("\n made it to FTDI initialize 2   \n");
		ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventDWord);
		FT_Purge(ftHandle, FT_PURGE_RX);
	}
}


