#pragma once
#include"Global.h"
#include"LinkList.h"


/////////////////////////////////////////DEFINE SECTION//////////////////////////////////////////////////

#define ONE_KB_SIZE					1024										//1024Bytes = 1KiloBytes
#define BLOCK_SIZE 					ONE_KB_SIZE									//Data block size which will actually hold the disk data 

#define MAX_NO_OF_BUF				1024 										//Max buf available in buf pool

#define MAX_NO_OF_BUF_PER_HASH_QUE	1024				 			 			//Linked to MAX_NO_OF_DISK_BLOCKS
#define MAX_NO_OF_DISK_BLOCKS		65536										//As ui16BlockNo is  UI16 hence 0 to 65535 hence max HDD size 65536KB = 64MB (considering 1 Block = 1024Bytes)
#define NO_OF_HASH_QUE				MAX_NO_OF_DISK_BLOCKS / MAX_NO_OF_BUF_PER_HASH_QUE	//Number of Hash Queues required to maintain buffers comes to be 64 
/*
1st hash que 0 to MAX_NO_OF_BUF_PER_HASH_QUE-1 ,
2nd hash que MAX_NO_OF_BUF_PER_HASH_QUE to MAX_NO_OF_BUF_PER_HASH_QUE X 2 - 1,
3rd hash que MAX_NO_OF_BUF_PER_HASH_QUE X 2 to MAX_NO_OF_BUF_PER_HASH_QUE X 3 - 1,
and so on.....
last hsh que MAX_NO_OF_BUF_PER_HASH_QUE X (NO_OF_HASH_QUE-1)  to MAX_NO_OF_DISK_BLOCKS - 1
*/

#define FREE_LIST	0					// Used as parameter for generic link list functions to indicate which link list
#define HASH_QUE	1					// Used as parameter for generic link list functions to indicate which link list

#define BEGINING	0					// Used as parameter for generic link list functions to indicate whr to add node on link list
#define END			1					// Used as parameter for generic link list functions to indicate whr to add node on link list

#define LOCKED		0x0001				// To lock OR with this
#define UNLOCK		(~(LOCKED))			// To unlock AND with this

#define VALID		0x0002				// To valid OR with this
#define INVALID		(~(VALID))			// To invalid AND with this

#define READING		0x0004				// When reading, OR with this
#define WRITING		0X0008				// When writing, OR with this

#define DELAYEDWR	0x0010				// When DELAYEDWR, OR with this
#define OLD			0x0020				// When DELAYEDWR done mark OLD by ORing with this

/////////////////////////////////////////DEFINE SECTION//////////////////////////////////////////////////




/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////
typedef struct BlockDataStruct
{

	UI8 ui8UniformByteStream[BLOCK_SIZE];	// Array which holds the actual disk data

}BLOCK_DATA_STRUCT;





typedef struct BufStruct
{
	UI16 ui16BlockNo;

	UI16 ui16BufStatus;									

	struct BlockDataStruct* pBufIncoreDiskData;			

	GENERIC_NODE_STRUCT LinkList[2];

}BUF_STRUCT;


/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////




extern BUF_STRUCT Buf[MAX_NO_OF_BUF]; 									// Actual buffers 						
extern BUF_STRUCT HashQueHeader[NO_OF_HASH_QUE];						// Header nodes for Hash Ques			 
extern BUF_STRUCT FreeLstHeader;										// Header node for free list

extern BLOCK_DATA_STRUCT ui8BufIncoreDiskBlockData[MAX_NO_OF_BUF];		// Array of struct which holds the actual disk data for each buffer

extern UI8 ui8GetBlk(UI16, BUF_STRUCT**);
extern UI8 ui8brelse(BUF_STRUCT*);
extern UI8 ui8bread(UI16 ui16InputBlockNo, BUF_STRUCT** ppReturnBuf);
extern UI8 ui8bwrite(BUF_STRUCT* pBufToBeWritten);
extern UI8 ui8AddBufferToLinkList(BUF_STRUCT*, BUF_STRUCT*, UI8);
extern UI8 ui8RemoveBufferFromLinkList(UI8, BUF_STRUCT*);
extern UI8 ui8InitBufPool(void);
extern UI8 ui8BlockNoToBUffer(UI16, BUF_STRUCT**);