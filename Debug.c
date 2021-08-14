#include"Debug.h"
#include"BufferCache.h"

FILE * fpDebugData;						// File pointer for debug log

UI8 ui8PrintBuffer(BUF_STRUCT* pBufToPrint, UI8 ui8TargetDevice, FILE* fpDebugFile);

UI8 ui8PrintBuffer(BUF_STRUCT* pBufToPrint, UI8 ui8TargetDevice, FILE* fpDebugFile)
{
	UI16 ui16LoopCounter;

	switch (ui8TargetDevice)
	{
		case DEBUG_FILE:
		{
			fprintf(fpDebugFile, "\n");
			fprintf(fpDebugFile, "ui16BlockNo: %d\n", pBufToPrint->ui16BlockNo);
			fprintf(fpDebugFile, "SelfAddress: %p\n", pBufToPrint);
			fprintf(fpDebugFile, "ui16BufStatus: %x\n", pBufToPrint->ui16BufStatus);
			fprintf(fpDebugFile, "pBufIncoreDiskData: %p\n", pBufToPrint->pBufIncoreDiskData);
			ui8PrintNode((GENERIC_NODE_STRUCT*)&(pBufToPrint->LinkList[FREE_LIST]), DEBUG_FILE, fpDebugFile);
			ui8PrintNode((GENERIC_NODE_STRUCT*)&(pBufToPrint->LinkList[HASH_QUE]), DEBUG_FILE, fpDebugFile);

			if (pBufToPrint->pBufIncoreDiskData)
			{
				for (ui16LoopCounter = 0; ui16LoopCounter < BLOCK_SIZE; ui16LoopCounter++)
				{
					if (!(ui16LoopCounter % 32))
					{
						fprintf(fpDebugFile, "\n");
					}
					fprintf(fpDebugFile, "%x ", pBufToPrint->pBufIncoreDiskData->ui8UniformByteStream[ui16LoopCounter]);
				}
				fprintf(fpDebugFile, "\n");
			}

			break;
		}
		case CONSOLE:
		{
			printf("\n");
			printf("ui16BlockNo: %d\n", pBufToPrint->ui16BlockNo);
			printf("SelfAddress: %p\n", pBufToPrint);
			printf("ui16BufStatus: %d\n", pBufToPrint->ui16BufStatus);
			printf("pBufIncoreDiskData: %p\n", pBufToPrint->pBufIncoreDiskData);
			ui8PrintNode((GENERIC_NODE_STRUCT*)&(pBufToPrint->LinkList[FREE_LIST]), CONSOLE, NULL);
			ui8PrintNode((GENERIC_NODE_STRUCT*)&(pBufToPrint->LinkList[HASH_QUE]), CONSOLE, NULL);

			if (pBufToPrint->pBufIncoreDiskData)
			{
				for (ui16LoopCounter = 0; ui16LoopCounter < BLOCK_SIZE; ui16LoopCounter++)
				{
					if (!(ui16LoopCounter % 32))
					{
						printf("\n");
					}
					printf("%x ", pBufToPrint->pBufIncoreDiskData->ui8UniformByteStream[ui16LoopCounter]);
				}
				printf("\n");
			}
			
			getchar();
			break;
		}
		default:
		{
			break;
		}
	}
	return SUCCESS;
}