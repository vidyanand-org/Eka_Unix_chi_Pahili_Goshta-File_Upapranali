#include"Main.h"
#include"BufferCache.h"
#include"Debug.h"
#include "HardDiskFileDriver.h"

SI16 main(void);


SI16 main(void)
{
	BUF_STRUCT* pBufLocalBlk;

	UI16 ui16MenuSelected;
	UI16 ui16InputBlockNo;	

	UI8 ui8ZeroValue = V_NULL;
//	UI16 ui16LoopCounter;

/*
	for (ui16LoopCounter = 0; ui16LoopCounter < MAX_NO_OF_BUF; ui16LoopCounter++)
	{
		ui8UnsafeMemoeryCopy((UC8*)&Buf[ui16LoopCounter], (UI32)1, (UC8*)&ui8ZeroValue, (UI32)(sizeof(BUF_STRUCT)));
	}
	
	for (ui16LoopCounter = 0; ui16LoopCounter < NO_OF_HASH_QUE; ui16LoopCounter++)
	{
		ui8UnsafeMemoeryCopy((UC8*)&HashQueHeader[ui16LoopCounter], (UI32)1, (UC8*)&ui8ZeroValue, (UI32)(sizeof(BUF_STRUCT)));
	}

	ui8UnsafeMemoeryCopy((UC8*)&FreeLstHeader, (UI32)1, (UC8*) &ui8ZeroValue, (UI32)( sizeof(BUF_STRUCT)));

	for (ui16LoopCounter = 0; ui16LoopCounter < MAX_NO_OF_BUF; ui16LoopCounter++)
	{
		ui8UnsafeMemoeryCopy((UC8*)&ui8BufIncoreDiskBlockData[ui16LoopCounter], (UI32)1, (UC8*)&ui8ZeroValue, (UI32)(sizeof(BLOCK_DATA_STRUCT)));
	}
*/

	ui8InitBufPool();

	while (TRUE)
	{
		printf("\n\n\nMENU");
		printf("\n0:QUIT");
		printf("\n1:getblk()");
		printf("\n2:brelease()");
		printf("\n3:bread()");
		printf("\n4:bwrite()");
		printf("\n5:FormateHardDiskFile()");
		printf("\n6:ui8PrintBuffer()");
		printf("\nEnter Your Choise: ");
		scanf_s("%d", &ui16MenuSelected);

		switch (ui16MenuSelected)
		{


		case 0:
			exit(0);
			break;


		case 1:
			printf("\nBlock No - ");
			scanf_s("%u", &ui16InputBlockNo);
			ui8GetBlk(ui16InputBlockNo, &pBufLocalBlk);
			ui8PrintBuffer(pBufLocalBlk, CONSOLE, NULL);
			break;


		case 2:
			printf("\nBlock No - ");
			scanf_s("%u", &ui16InputBlockNo);
			ui8BlockNoToBUffer(ui16InputBlockNo, &pBufLocalBlk);
			if (pBufLocalBlk)
			{
				ui8brelse(pBufLocalBlk);
				ui8PrintBuffer(pBufLocalBlk, CONSOLE, NULL);
			}
			else
			{
				printf("\nBlock not present in buf pool");
			}
			break;


		case 3:
			printf("\nBlock No - ");
			scanf_s("%u", &ui16InputBlockNo);
			ui8bread(ui16InputBlockNo, &pBufLocalBlk);
			ui8bwrite(pBufLocalBlk);
			ui8PrintBuffer(pBufLocalBlk, CONSOLE, NULL);
			break;


		case 4:
			printf("\nBlock No - ");
			scanf_s("%u", &ui16InputBlockNo);
			ui8bread(ui16InputBlockNo, &pBufLocalBlk);

			ui8UnsafeMemoeryCopy((UC8*)"A", (UI32)1,
				(UC8*)(pBufLocalBlk->pBufIncoreDiskData->ui8UniformByteStream), (UI32)BLOCK_SIZE);

			ui8bwrite(pBufLocalBlk);
			ui8PrintBuffer(pBufLocalBlk, CONSOLE, NULL);
			break;


		case 5:
			ui8FormatHardDiskFile();
			ui8InitBufPool();
			break;

		case 6:
			printf("\nDataBuff/SpecialBuf - 0/1 : ");
			scanf_s("%u", &ui16InputBlockNo);
			if (ui16InputBlockNo == 0)
			{
				printf("\nDataBuff  0-1023: ");
				scanf_s("%u", &ui16InputBlockNo);
				if (ui16InputBlockNo <= 1023)
				{
					ui8PrintBuffer(&Buf[ui16InputBlockNo], CONSOLE, NULL);
				}
				else
				{
					printf("\nInvalid Input");
				}
				
			}
			else if (ui16InputBlockNo == 1)
			{
				printf("\nSpecialBuff  0-64 (64-Free): ");
				scanf_s("%u", &ui16InputBlockNo);
				if (ui16InputBlockNo <= 63)
				{
					ui8PrintBuffer(&HashQueHeader[ui16InputBlockNo], CONSOLE, NULL);
				}
				else if (ui16InputBlockNo == 64)
				{
					ui8PrintBuffer(&FreeLstHeader, CONSOLE, NULL);
				}
				else
				{
					printf("\nInvalid Input");
				}

			}
			else
			{
				printf("\nInvalid Input");
			}
			break;

		default:
			printf("\nInvalid Input");
			break;
		}

	}
	return 0;
}