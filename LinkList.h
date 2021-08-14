#pragma once
#include"Global.h"

#define TYPE_BUF_STRUCT				0		// Used when operation is to be performed on linked list of buffer 
#define TYPE_INCORE_INODE_STRUCT	1		// Used when operation is to be performed on linked list of inodes

#define	DEBUG_FILE					0		// For where to print the data
#define CONSOLE						1		// For where to print the data
/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////
typedef struct GenericNodeStruct
{
	struct GenericNodeStruct* pPrevNode;	// Pointer to previous GenericNodeStruct in link list
	struct GenericNodeStruct* pNextNode;	// Pointer to next GenericNodeStruct in link list
	void* pSelfAddr;						// Pointer to the main structure which is held in linklist by GenericNodeStruct, hence void

}GENERIC_NODE_STRUCT;
/////////////////////////////////////////TYPEDEF SECTION//////////////////////////////////////////////////


extern UI8 ui8InitLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
extern UI8 ui8RemoveNodeFromLinkList(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
extern UI8 ui8InsertNodeToLinkList(void* pUnknownStruct, void* pUnknownStructPrev, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
extern UI8 ui8RemoveNode(GENERIC_NODE_STRUCT* pNodeToRemove);
extern UI8 ui8InsertNode(GENERIC_NODE_STRUCT* pNodeToInsert, GENERIC_NODE_STRUCT* pPrevNodeTopNodeToInsert);
extern UI8 ui8PrintNode(GENERIC_NODE_STRUCT* pNodeToPrint, UI8 ui8TargetDevice, FILE* fpDebugFile);
extern UI8 ui8CreateSelfPointingNode(GENERIC_NODE_STRUCT* pNode);
extern UI8 ui8ClearLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
extern UI8 ui8ClearLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
extern UI8 ui8ClearNode(GENERIC_NODE_STRUCT* pNode);
extern UI8 ui8BreakNode(GENERIC_NODE_STRUCT* pNodeToBreak);
extern UI8 ui8InitNode(GENERIC_NODE_STRUCT* pNode, void* pUnknownStruct);
extern UI8 ui8InitLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);