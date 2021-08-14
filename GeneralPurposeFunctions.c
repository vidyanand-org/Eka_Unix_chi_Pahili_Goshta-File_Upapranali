#include"GeneralPurposeFunctions.h"




UI8 ui8UnsafeMemoeryCopy(UC8* uc8pSource, UI32 ui32NoOfSourceBytes, UC8* uc8pDestination, UI32 ui32NoOfDestinationBytes)
{
	UC8* uc8pSourceStartBackup;
	UI32 ui32NoOfSourceBytesBackup;

	uc8pSourceStartBackup = uc8pSource;
	ui32NoOfSourceBytesBackup = ui32NoOfSourceBytes;

	while(ui32NoOfDestinationBytes)
	{
		(UC8)(*uc8pDestination) = (UC8)(*uc8pSource);

		ui32NoOfDestinationBytes--;
		ui32NoOfSourceBytes--;
		uc8pDestination++;
		uc8pSource++;

		if (ui32NoOfSourceBytes == 0)
		{
			ui32NoOfSourceBytes = ui32NoOfSourceBytesBackup;
			uc8pSource = uc8pSourceStartBackup;
		}
	}

	return 0;
}