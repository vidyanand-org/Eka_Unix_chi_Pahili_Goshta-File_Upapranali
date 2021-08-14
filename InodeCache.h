#pragma once
#include"Global.h"
#include"LinkList.h"
#include"HardDiskFileDriver.h"


/////////////////////////////////////////DEFINE SECTION//////////////////////////////////////////////////

#define TOTAL_IN_CORE_INODE				1024 										//Max inode available in inode pool

#define MAX_NO_OF_INODE_PER_HASH_QUE	64				 			 				//Linked to TOTAL_ON_DISK_INODE

#define NO_OF_INODE_HASH_QUE			TOTAL_ON_DISK_INODE / MAX_NO_OF_INODE_PER_HASH_QUE	//Number of Hash Queues required to maintain inodes comes to be 64 
/*
1st hash que 0 to MAX_NO_OF_INODE_PER_HASH_QUE-1 ,
2nd hash que MAX_NO_OF_INODE_PER_HASH_QUE to MAX_NO_OF_INODE_PER_HASH_QUE X 2 - 1,
3rd hash que MAX_NO_OF_INODE_PER_HASH_QUE X 2 to MAX_NO_OF_INODE_PER_HASH_QUE X 3 - 1,
and so on.....
last hsh que MAX_NO_OF_INODE_PER_HASH_QUE X (NO_OF_INODE_HASH_QUE-1)  to TOTAL_ON_DISK_INODE - 1
*/




#define FILE_TYPE_DATA		0x00	
#define FILE_TYPE_DIR		0x01



#define PERMIT_READ			0x0001				// To allow reading OR with this
#define PROTECT_READ		(~(PERMIT_READ))	// To block reading AND with this	

#define PERMIT_WRITE		0x0002				// To allow writing OR with this
#define PROTECT_WRITE		(~(PERMIT_WRITE))	// To block writing AND with this

#define PERMIT_EXECUTE		0x0004					// To allow executing OR with this
#define PROTECT_EXECUTE		(~(PERMIT_EXECUTE))		// To block executing AND with this



#define LOCKED				0x0001				// To lock OR with this
#define UNLOCK				(~(LOCKED))			// To unlock AND with this

#define VALID				0x0002				// To valid OR with this
#define INVALID				(~(VALID))			// To invalid AND with this

#define RESERVED			0x0004				// To mark as reserved OR with this
#define UNRESERVED			(~(RESERVED))		// To mark as unreserved AND with this

#define INODE_MODIFIED		0x0008				// To mark as inode modified OR with this
#define INODE_UNMODIFIED	(~(INODE_MODIFIED))	// To mark as inode unmodified AND with this

#define DATA_MODIFIED		0x0010				// To mark as data modified OR with this
#define DATA_UNMODIFIED		(~(DATA_MODIFIED))	// To mark as data unmodified AND with this

/////////////////////////////////////////DEFINE SECTION//////////////////////////////////////////////////





/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////

typedef struct DiskInodeStruct
{
	UI32 ui32FileSizeInBytes;
	UI8 ui8FileType; 
	UI8 ui8LinkCount;
	UI16 ui16FilePermission;
	UI16 ui16DiskBlockNo[13];
	
}DISK_INODE_STRUCT;


typedef struct InCoreInodeDataStruct
{
	UI16 ui16InodeNo;
	UI16 ui16InodeStatus;
	UI8 ui8ReferenceCount;
	GENERIC_NODE_STRUCT LinkList[2];

}INCORE_INODE_DATA_STRUCT;


typedef struct InCoreInodeStruct
{
	DISK_INODE_STRUCT DiskInodeData;
	INCORE_INODE_DATA_STRUCT IncoreInodeData;

}INCORE_INODE_STRUCT;

/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////


extern INCORE_INODE_STRUCT Inode[TOTAL_IN_CORE_INODE]; 					// Actual incore inodes 						
extern INCORE_INODE_STRUCT InodeHashQueHeader[NO_OF_INODE_HASH_QUE];	// Header nodes for Hash Ques			 
extern INCORE_INODE_STRUCT InodeFreeLstHeader;							// Header node for free list
