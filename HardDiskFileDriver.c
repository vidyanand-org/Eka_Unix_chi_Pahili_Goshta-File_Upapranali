#include"HardDiskFileDriver.h"
#include"BufferCache.h"

UI8 ui8FormatHardDiskFile(void);
UI8 ui8ReadBlockFromHardDiskFile(BUF_STRUCT* pBufToFill);
UI8 ui8WriteBlockToHardDiskFile(BUF_STRUCT* pBufToEmpty);


UI8 ui8FormatHardDiskFile(void)
{
	FILE * fpHardDiskFile;						// File pointer for Hard Disk File
	BUF_STRUCT* pLocalBufStruct;
	UI16 ui16LocalLoopCounter;

	fopen_s(&fpHardDiskFile, "HardDiskFile.bin", "wb");


	for (ui16LocalLoopCounter = 0; ui16LocalLoopCounter < MAX_NO_OF_DISK_BLOCKS; ui16LocalLoopCounter++)
	{
		ui8GetBlk(ui16LocalLoopCounter, &pLocalBufStruct);

		ui8UnsafeMemoeryCopy((UC8*)&ui16LocalLoopCounter, (UI32)2,
			(UC8*) (pLocalBufStruct->pBufIncoreDiskData->ui8UniformByteStream), (UI32)BLOCK_SIZE);

		fwrite(pLocalBufStruct->pBufIncoreDiskData, BLOCK_SIZE, 1, fpHardDiskFile);

		ui8brelse(pLocalBufStruct);
	}

	fclose(fpHardDiskFile);

	return 0;
}


UI8 ui8ReadBlockFromHardDiskFile(BUF_STRUCT* pBufToFill)
{
	FILE * fpHardDiskFile;						

	pBufToFill->ui16BufStatus = pBufToFill->ui16BufStatus | (READING);

	fopen_s(&fpHardDiskFile, "HardDiskFile.bin", "rb");
	fseek(fpHardDiskFile, (pBufToFill->ui16BlockNo * BLOCK_SIZE), SEEK_SET);
	fread(pBufToFill->pBufIncoreDiskData, BLOCK_SIZE, 1, fpHardDiskFile);
	fclose(fpHardDiskFile);

	pBufToFill->ui16BufStatus = pBufToFill->ui16BufStatus | VALID;
	pBufToFill->ui16BufStatus = pBufToFill->ui16BufStatus & (~READING);

	return 0;
}


UI8 ui8WriteBlockToHardDiskFile(BUF_STRUCT* pBufToEmpty)
{
	FILE * fpHardDiskFile;						// File pointer for Hard Disk File

	pBufToEmpty->ui16BufStatus = pBufToEmpty->ui16BufStatus | (WRITING);

	fopen_s(&fpHardDiskFile, "HardDiskFile.bin", "rb+");
	fseek(fpHardDiskFile, (pBufToEmpty->ui16BlockNo * BLOCK_SIZE), SEEK_SET);
	fwrite(pBufToEmpty->pBufIncoreDiskData, BLOCK_SIZE, 1, fpHardDiskFile);
	fclose(fpHardDiskFile);

	pBufToEmpty->ui16BufStatus = pBufToEmpty->ui16BufStatus & (~WRITING);

	return 0;
}