#include"LinkList.h"			
#include"BufferCache.h"
#include"InodeCache.h"



UI8 ui8InitLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
UI8 ui8RemoveNodeFromLinkList(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
UI8 ui8InsertNodeToLinkList(void* pUnknownStruct, void* pUnknownStructPrev, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
UI8 ui8RemoveNode(GENERIC_NODE_STRUCT* pNodeToRemove);
UI8 ui8InsertNode(GENERIC_NODE_STRUCT* pNodeToInsert, GENERIC_NODE_STRUCT* pPrevNodeTopNodeToInsert );
UI8 ui8PrintNode(GENERIC_NODE_STRUCT* pNodeToPrint, UI8 ui8TargetDevice, FILE* fpDebugFile);
UI8 ui8CreateSelfPointingNode(GENERIC_NODE_STRUCT* pNode);
UI8 ui8ClearLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
UI8 ui8ClearLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);
UI8 ui8ClearNode(GENERIC_NODE_STRUCT* pNode);
UI8 ui8BreakNode(GENERIC_NODE_STRUCT* pNodeToBreak);
UI8 ui8InitNode(GENERIC_NODE_STRUCT* pNode, void* pUnknownStruct);
UI8 ui8InitLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex);



UI8 ui8RemoveNodeFromLinkList(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{
	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:
		ui8RemoveNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStruct)->LinkList[ui8LinkListIndex]));
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8RemoveNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStruct)->IncoreInodeData.LinkList[ui8LinkListIndex]));
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8InsertNodeToLinkList(void* pUnknownStruct, void* pUnknownStructPrev, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{

	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:
		ui8InsertNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStruct)->LinkList[ui8LinkListIndex]), (GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStructPrev)->LinkList[ui8LinkListIndex]));
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8InsertNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStruct)->IncoreInodeData.LinkList[ui8LinkListIndex]), (GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStructPrev)->IncoreInodeData.LinkList[ui8LinkListIndex]));
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8InitLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{
	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:
		ui8InitNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStruct)->LinkList[ui8LinkListIndex]), pUnknownStruct);
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8InitNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStruct)->IncoreInodeData.LinkList[ui8LinkListIndex]), pUnknownStruct);
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8ClearLinkListNode(void* pUnknownStruct, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{
	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:
		ui8ClearNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStruct)->LinkList[ui8LinkListIndex]));
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8ClearNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStruct)->IncoreInodeData.LinkList[ui8LinkListIndex]));
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8InitLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{
	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:	
		ui8CreateSelfPointingNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStructHeader)->LinkList[ui8LinkListIndex]));
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8CreateSelfPointingNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStructHeader)->IncoreInodeData.LinkList[ui8LinkListIndex]));
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8ClearLinkListHeader(void* pUnknownStructHeader, UI8 ui8TypeofpUnknownStruct, UI8 ui8LinkListIndex)
{
	switch (ui8TypeofpUnknownStruct)
	{
	case TYPE_BUF_STRUCT:
		ui8BreakNode((GENERIC_NODE_STRUCT*)&(((BUF_STRUCT*)pUnknownStructHeader)->LinkList[ui8LinkListIndex]));
		break;

	case TYPE_INCORE_INODE_STRUCT:
		ui8BreakNode((GENERIC_NODE_STRUCT*)&(((INCORE_INODE_STRUCT*)pUnknownStructHeader)->IncoreInodeData.LinkList[ui8LinkListIndex]));
		break;

	default:
		break;
	}

	return SUCCESS;
}



UI8 ui8InitNode(GENERIC_NODE_STRUCT* pNode, void* pUnknownStruct)
{
	pNode->pSelfAddr = pUnknownStruct;

	return SUCCESS;
}



UI8 ui8ClearNode(GENERIC_NODE_STRUCT* pNode)
{
	pNode->pSelfAddr = P_NULL;

	return SUCCESS;
}



UI8 ui8CreateSelfPointingNode(GENERIC_NODE_STRUCT* pNode)
{
	pNode->pNextNode = pNode;
	pNode->pPrevNode = pNode;

	return SUCCESS;
}



UI8 ui8BreakNode(GENERIC_NODE_STRUCT* pNodeToBreak)
{
	pNodeToBreak->pNextNode = P_NULL;
	pNodeToBreak->pPrevNode = P_NULL;

	return SUCCESS;
}



UI8 ui8RemoveNode(GENERIC_NODE_STRUCT* pNodeToRemove)
{
	GENERIC_NODE_STRUCT* pPrevNodeTopNodeToRemove;
	GENERIC_NODE_STRUCT* pNextNodeTopNodeToRemove;

	pPrevNodeTopNodeToRemove = (GENERIC_NODE_STRUCT*)(pNodeToRemove->pPrevNode);
	pNextNodeTopNodeToRemove = (GENERIC_NODE_STRUCT*)(pNodeToRemove->pNextNode);

	pPrevNodeTopNodeToRemove->pNextNode = (GENERIC_NODE_STRUCT*)pNextNodeTopNodeToRemove;
	pNextNodeTopNodeToRemove->pPrevNode = (GENERIC_NODE_STRUCT*)pPrevNodeTopNodeToRemove;

	ui8BreakNode((GENERIC_NODE_STRUCT*)pNodeToRemove);

	return SUCCESS;
}



UI8 ui8InsertNode(GENERIC_NODE_STRUCT* pNodeToInsert, GENERIC_NODE_STRUCT* pPrevNodeTopNodeToInsert)
{
	GENERIC_NODE_STRUCT* pNextNodeTopNodeToInsert;

	pNextNodeTopNodeToInsert = (GENERIC_NODE_STRUCT*)pPrevNodeTopNodeToInsert->pNextNode;

	pNodeToInsert->pNextNode = pNextNodeTopNodeToInsert;
	pNodeToInsert->pPrevNode = pPrevNodeTopNodeToInsert;

	pPrevNodeTopNodeToInsert->pNextNode = pNodeToInsert;
	pNextNodeTopNodeToInsert->pPrevNode = pNodeToInsert;

	return SUCCESS;
}



UI8 ui8PrintNode(GENERIC_NODE_STRUCT* pNodeToPrint, UI8 ui8TargetDevice, FILE* fpDebugFile)
{
	switch (ui8TargetDevice)
	{
		case DEBUG_FILE:
		{
			fprintf(fpDebugFile, "\n");
			fprintf(fpDebugFile, "pNodeToPrint: %p\n", pNodeToPrint);
			fprintf(fpDebugFile, "pSelfAddr: %p\n", pNodeToPrint->pSelfAddr);
			fprintf(fpDebugFile, "pNextNode: %p\n", pNodeToPrint->pNextNode);
			fprintf(fpDebugFile, "pPrevNode: %p\n", pNodeToPrint->pPrevNode);
			break;
		}
		case CONSOLE:
		{
			printf("\n");
			printf( "pNodeToPrint: %p\n", pNodeToPrint);
			printf("pSelfAddr: %p\n", pNodeToPrint->pSelfAddr);
			printf("pNextNode: %p\n", pNodeToPrint->pNextNode);
			printf("pPrevNode: %p\n", pNodeToPrint->pPrevNode);
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