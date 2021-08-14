#include"InodeCache.h"			



INCORE_INODE_STRUCT Inode[TOTAL_IN_CORE_INODE]; 				// Actual incore inodes 						
INCORE_INODE_STRUCT InodeHashQueHeader[NO_OF_INODE_HASH_QUE];	// Header nodes for Hash Ques			 
INCORE_INODE_STRUCT InodeFreeLstHeader;							// Header node for free list



UI8 ui8GetInode(UI16, INCORE_INODE_STRUCT**);
UI8 ui8InodeNoToInode(UI16, INCORE_INODE_STRUCT**);
UI8 ui8RemoveInodeFromLinkList(UI8 , INCORE_INODE_STRUCT* );
UI8 ui8AddInodeToLinkList(INCORE_INODE_STRUCT* , INCORE_INODE_STRUCT* , UI8 );
UI8 ui8ReadInode(UI16, INCORE_INODE_STRUCT*);


UI8 ui8GetInode(UI16 ui16RequestedInodeNo, INCORE_INODE_STRUCT** ppReturnInode)
{
	INCORE_INODE_STRUCT* pAllotedInode = NULL;		//Local structure pointer 

	while (TRUE)	
	{
		//Below function search the inode with corresponding inode no already present in inode pool ie Hash Queue search
		ui8InodeNoToInode(ui16RequestedInodeNo, &pAllotedInode);

		if (pAllotedInode) 	// As pAllotedInode not NULL inode present on Hash Que
		{

			// As the requested inode no inode is on hash que check its status
			if (pAllotedInode->IncoreInodeData.ui16InodeStatus & LOCKED)
			{
				printf("\nTHIS Process sleeping for THIS inode in while(1)...");
				while (1);
				//RECEIVED SIGNAL ### FROM PutInode()
				pAllotedInode = NULL;	// Clear the inode pointer for next trip just for safety
				continue;				// Continue the search from begining
			}
			else // The inode is found on hash que and is not lock hence the inode is ready to use
			{
				// Check that the inode is on free list or not
				if (pAllotedInode->IncoreInodeData.ui8ReferenceCount)
				{
					// Do nothing
				}
				else
				{
					// As the inode is on free list hence remove it
					ui8RemoveInodeFromLinkList(FREE_LIST, pAllotedInode);
				}
				// Increment the ref count as now it is reffered by demanding process
				pAllotedInode->IncoreInodeData.ui8ReferenceCount++;

				// Lock the inode as it is aquired for some updation 
				pAllotedInode->IncoreInodeData.ui16InodeStatus = (pAllotedInode->IncoreInodeData.ui16InodeStatus | LOCKED);
				
				//Break the infinite while loop as inode is acquired
				break;
			}

		}
		else // As pAllotedInode is null means inode not on hash que
		{
			// Is there any free inode present?
			if (InodeFreeLstHeader.IncoreInodeData.ui16InodeNo == 0)
			{
				// All existing inodes are aquired very rare condition 
				pAllotedInode = NULL;
				break;
			}
			else
			{

				//Allocate the frist present inode on free list
				pAllotedInode = ((GENERIC_NODE_STRUCT*)(InodeFreeLstHeader.IncoreInodeData.LinkList[FREE_LIST].pNextNode))->pSelfAddr;

				// Lock the inode as it is now under use
				pAllotedInode->IncoreInodeData.ui16InodeStatus = (pAllotedInode->IncoreInodeData.ui16InodeStatus | LOCKED);

				// Remove allocated frist present inode from free list
				ui8RemoveInodeFromLinkList(FREE_LIST, pAllotedInode);

				// Set the ref count as now it is reffered by demanding process
				pAllotedInode->IncoreInodeData.ui8ReferenceCount = 1;

				// Remove the inode from the old(present) hash que	
				ui8RemoveInodeFromLinkList(HASH_QUE, pAllotedInode);

				// Initialize the ui16InodeNo of that inode with requested inode no - KAPALAVARCHE PUSUN NAVIN LIHINE
				pAllotedInode->IncoreInodeData.ui16InodeNo = ui16RequestedInodeNo;

				// Mark the inode as invaild as it is taken from free list and relocated on hash que
				pAllotedInode->IncoreInodeData.ui16InodeStatus = (pAllotedInode->IncoreInodeData.ui16InodeStatus  & INVALID);

				// Add the inode to the new hash que as per the received inode no
				ui8AddInodeToLinkList(&InodeHashQueHeader[ui16RequestedInodeNo / MAX_NO_OF_INODE_PER_HASH_QUE], pAllotedInode, BEGINING);
	
				ui8ReadInode(ui16RequestedInodeNo, pAllotedInode);

				//Break the infinite while loop as buf is acquired
				break;

			}
				
		}

	}
	// Fill the out parameter for the calling function with the address of acquired inode
	*ppReturnInode = pAllotedInode;

	if (pAllotedInode)
	{
		return SUCCESS;
	}
	else
	{
		return FAIL;
	}
}



UI8 ui8InodeNoToInode(UI16 ui16InputInodeNo, INCORE_INODE_STRUCT** ppReturnInode)
{
	UI16 ui16LoopCounter;
	void* pLocalInode;

	// Find the hash que for the requested inode
	// ui16InputInodeNo / MAX_NO_OF_INODE_PER_HASH_QUE is that hash que 		
	// Assign first node on that hash que to satrt with 
	pLocalInode = InodeHashQueHeader[ui16InputInodeNo / MAX_NO_OF_INODE_PER_HASH_QUE].IncoreInodeData.LinkList[HASH_QUE].pNextNode;
	pLocalInode = ((GENERIC_NODE_STRUCT*)pLocalInode)->pSelfAddr;
	// Header ui16InodeNo contains the present count of nodes on that link list
	ui16LoopCounter = InodeHashQueHeader[ui16InputInodeNo / MAX_NO_OF_INODE_PER_HASH_QUE].IncoreInodeData.ui16InodeNo;

	while (ui16LoopCounter)			// Search till whole linklist is not searched
	{
		// Is the current nodes inode no matches with the inode no which is received
		if (((INCORE_INODE_STRUCT*)pLocalInode)->IncoreInodeData.ui16InodeNo == ui16InputInodeNo)
		{
			break;		// break the loop as the node is found
		}
		else
		{
			// Go to the next node as the current nodes inode no does not matched with inode no requested
			pLocalInode = ((INCORE_INODE_STRUCT*)pLocalInode)->IncoreInodeData.LinkList[HASH_QUE].pNextNode;
			pLocalInode = ((GENERIC_NODE_STRUCT*)pLocalInode)->pSelfAddr;
			ui16LoopCounter--; // decrement the loopcounter to exit loop at end of link list
		}
	}
	// As loop counter is zero indicates required inode not found on hash que 
	// and control comes out of while due to end of link list
	if (ui16LoopCounter == 0)
	{
		pLocalInode = NULL; // Assign NULL to the pointer which is to be returned as inode not found
	}

	*ppReturnInode = pLocalInode; //Fill the out parameter
	return 0;

}



UI8 ui8AddInodeToLinkList(INCORE_INODE_STRUCT* pHead, INCORE_INODE_STRUCT* pInodeToBeAdded, UI8 ui8Location)
{
	// As free list pointer is not NULL, this indicates that it is free list header 
	//and the inode is to be added on Free List
	if (pHead->IncoreInodeData.LinkList[FREE_LIST].pNextNode != NULL)
	{
		if (ui8Location == BEGINING)
		{
			ui8InsertNodeToLinkList((void*)pInodeToBeAdded, (void*)pHead, TYPE_INCORE_INODE_STRUCT, FREE_LIST);
		}
		else if (ui8Location == END)
		{
			ui8InsertNodeToLinkList((void*)pInodeToBeAdded, (void*)((GENERIC_NODE_STRUCT*)(pHead->IncoreInodeData.LinkList[FREE_LIST].pPrevNode))->pSelfAddr, TYPE_INCORE_INODE_STRUCT, FREE_LIST);
		}
	}
	// As free list pointer is NULL, this indicates that it is hash que header 
	//and the inode is to be added on Hash Que, add at the begining
	else
	{
		ui8InsertNodeToLinkList((void*)pInodeToBeAdded, (void*)pHead, TYPE_INCORE_INODE_STRUCT, HASH_QUE);
	}
	// As the new node is added on link list increment the count of nodes on link list
	// Header ui16InodeNo contains the present count of nodes on that link list
	pHead->IncoreInodeData.ui16InodeNo = pHead->IncoreInodeData.ui16InodeNo + 1;

	return 0;
}



UI8 ui8RemoveInodeFromLinkList(UI8 ui8LinkList, INCORE_INODE_STRUCT* pRemovedInode)
{
	if (ui8LinkList == FREE_LIST)
	{

		ui8RemoveNodeFromLinkList((void*)pRemovedInode, TYPE_INCORE_INODE_STRUCT, FREE_LIST);

		// As the new node is removed from link list decrement the count of nodes on link list
		// Header ui16InodeNo contains the present count of nodes on that link list
		InodeFreeLstHeader.IncoreInodeData.ui16InodeNo--;
	}
	else if (ui8LinkList == HASH_QUE)
	{
		ui8RemoveNodeFromLinkList((void*)pRemovedInode, TYPE_INCORE_INODE_STRUCT, HASH_QUE);

		// As the new node is removed from link list decrement the count of nodes on link list
		// Header ui16InodeNo contains the present count of nodes on that link list
		// Find the hash que for the requested inode
		// pRemovedInode->IncoreInodeData.ui16InodeNo / MAX_NO_OF_INODE_PER_HASH_QUE gives the hash que haeder
		InodeHashQueHeader[pRemovedInode->IncoreInodeData.ui16InodeNo / MAX_NO_OF_BUF_PER_HASH_QUE].IncoreInodeData.ui16InodeNo--;
	}
	return 0;
}


UI8 ui8ReadInode(UI16 ui16InodeNoToRead, INCORE_INODE_STRUCT* pAllotedInode)
{
	UI16 ui16BlockNoToRead;
	BUF_STRUCT* pBufLocalBlk;
	UI16 ui16ByteOffsetOfInodeData;

	ui16BlockNoToRead = ((ui16InodeNoToRead - 1) / ON_DISK_INODE_PER_BLOCK) + DILB_BLOCK_START_ADDRESS;

	ui8bread(ui16BlockNoToRead, &pBufLocalBlk);

	ui16ByteOffsetOfInodeData = (((ui16InodeNoToRead - 1) % ON_DISK_INODE_PER_BLOCK) * ON_DISK_INODE_SIZE);

	ui8UnsafeMemoeryCopy((UC8*) ((pBufLocalBlk->pBufIncoreDiskData->ui8UniformByteStream) + ui16ByteOffsetOfInodeData), (UI32)sizeof(DISK_INODE_STRUCT), (UC8*)(&(pAllotedInode->DiskInodeData)), (UI32)sizeof(DISK_INODE_STRUCT));

	ui8brelse(pBufLocalBlk);

	return 0;
}