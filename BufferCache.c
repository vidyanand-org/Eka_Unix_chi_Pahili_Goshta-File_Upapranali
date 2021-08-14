#include"BufferCache.h"			
#include"Debug.h"
#include"HardDiskFileDriver.h"

BUF_STRUCT Buf[MAX_NO_OF_BUF]; 						// Actual buffers 						
BUF_STRUCT HashQueHeader[NO_OF_HASH_QUE];			// Header nodes for Hash Ques			 
BUF_STRUCT FreeLstHeader;							// Header node for free list

BLOCK_DATA_STRUCT ui8BufIncoreDiskBlockData[MAX_NO_OF_BUF];  // Array of struct which holds the actual disk data for each buffer

UI8 ui8GetBlk(UI16, BUF_STRUCT**);
UI8 ui8brelse(BUF_STRUCT*);
UI8 ui8AddBufferToLinkList(BUF_STRUCT*, BUF_STRUCT*, UI8);
UI8 ui8RemoveBufferFromLinkList(UI8, BUF_STRUCT*);
UI8 ui8InitBufPool(void);
UI8 ui8BlockNoToBUffer(UI16, BUF_STRUCT**);


/*
This function receives the block no which is needed to be read.
the function returns the buffer pointer which is allocated for 
the requested block no. The return value is out parameter (RIKAME BHANDE)
The structure pointed by the return pointer may contain valid 
or invalid data. It is responsiblity of the calling function to validate it.
*/
UI8 ui8GetBlk(UI16 ui16RequestedBlockNo, BUF_STRUCT** ppReturnBuf)
{
	BUF_STRUCT* pAllotedBuff = NULL;		//Local structure pointer 
#ifdef DEBUG
	UI16 ui16LoopCounter;			
	UI16 ui16MenuSelected;
#endif
	while (TRUE)	//infinite loop breaks when buffer gets alloted for requested block no
	{
		//Below function search the buffer with corresponding block no already present in buffer pool ie Hash Queue search
		ui8BlockNoToBUffer(ui16RequestedBlockNo, &pAllotedBuff);

		if (pAllotedBuff) 	// As pAllotedBuff not NULL buf present on Hash Que
		{
			// As the requested block no buf is on hash que check its status
			if (pAllotedBuff->ui16BufStatus & LOCKED) 
			{
#ifdef DEBUG
				printf("\nBuffer LOCKED and on HASH QUE");
				printf("\nTHIS Process sleeping for THIS buffer...");
				//sleep(); // Actual OS sleep function should get called here

				//Virtual scheduling code starts here------->
				printf("\nSimulate release of THIS bufer by OTHER Process?1/0 - ");
				scanf_s("%d", &ui16MenuSelected);
				if (ui16MenuSelected)
				{
					printf("\nOTHER Process scheduled which relesing THIS buffer");
					ui8brelse(pAllotedBuff);
				}
				printf("\nTHIS process is woken up from sleep for THIS buffer...");
				printf("\nTHIS Process Continuing...");
#else
				printf("\nTHIS Process sleeping for THIS buffer in while(1)...");
				while (1);
#endif
				//Virtual scheduling code ends here------->

				//RECEIVED SIGNAL ### FROM Brelease()
				pAllotedBuff = NULL;	// Clear the buf pointer for next trip just for safety
				continue;				// Continue the search from begining
			}
			else	// The buf is found on hash que and is not lock hence the buf is ready to use
			{
#ifdef DEBUG
				printf("\nBuffer FREE and on HASH QUE");
#endif
				// As the buf is not locked it must be on free list hence remoe it
				ui8RemoveBufferFromLinkList(FREE_LIST, pAllotedBuff); 
				// hence lock the buffer as it is now under use and removed from free list
				pAllotedBuff->ui16BufStatus = pAllotedBuff->ui16BufStatus | (LOCKED);
				break;		//Break the infinite while loop as buf is acquired
			}
		}
		else	// As pAllotedBuff is null means buf not on hash que
		{
			//Hearers ui16BlockNo contains the present count of nodes on that link list 
			//Check that is free list empty?
			if (FreeLstHeader.ui16BlockNo == 0) 					
			{
#ifdef DEBUG
				printf("\nFREE LIST EMPTY");
				printf("\nTHIS Process sleeping for ANY buffer...");
				//sleep(); // Actual OS sleep function should get called here

				//Virtual scheduling code starts here------->
				printf("\nSimulate release of ANY bufer by OTHER Process?1/0 - ");
				scanf_s("%d", &ui16MenuSelected);
				if (ui16MenuSelected)
				{
					printf("\nOTHER Process scheduled which relesing ANY buffer");
					// As no buf on free list, check all hash que from hash que 0 to release ANY buffer
					for (ui16LoopCounter = 0; ui16LoopCounter < NO_OF_HASH_QUE; ui16LoopCounter++)
					{
						//Hearers ui16BlockNo contains the present count of nodes on that link list
						if (HashQueHeader[ui16LoopCounter].ui16BlockNo != 0)	
						{
							// As the current serched hash que contains the buf on link list break
							break;		
						}
					}
					//Release the first buffer ( Which can be ANY buffer) present on the found hash que
					ui8brelse((BUF_STRUCT*)(((GENERIC_NODE_STRUCT*)HashQueHeader[ui16LoopCounter].LinkList[HASH_QUE].pNextNode)->pSelfAddr));
				}
				printf("\nTHIS process is woken up from sleep for ANY buffer...");
				printf("\nTHIS Process Continuing...");
				//Virtual scheduling code ends here------->
#else
				printf("\nTHIS Process sleeping for ANY buffer in while(1)...");
				while (1);
#endif
				//RECEIVED SIGNAL @@@ FROM Brelease()
				pAllotedBuff = NULL;	// Clear the buf pointer for next trip just for safety
				continue;				// Continue the search from begining
			}
			else	// Free list not empty 
			{
				//Allocate the frist present buf on free list
				pAllotedBuff = ((GENERIC_NODE_STRUCT*)(FreeLstHeader.LinkList[FREE_LIST].pNextNode))->pSelfAddr;
				// Remove allocated frist present buf from free list
				ui8RemoveBufferFromLinkList(FREE_LIST, pAllotedBuff);	 		
				// Is that buf t is marked as delayed write
				if (pAllotedBuff->ui16BufStatus & DELAYEDWR)				
				{
					// Mark the buffer as a writing to the disk, 
					// also Mark the buffer as a as busy as writing is initiated
					pAllotedBuff->ui16BufStatus = pAllotedBuff->ui16BufStatus | (LOCKED | WRITING);
					// Initiate the disk write operation here
#ifdef DEBUG
					printf("\nBuffer FREE but DELAYED WRITE");
					printf("\nTHIS Process Continuing...");
#endif
					pAllotedBuff = NULL;// Clear the buf pointer for next trip just for safety
					continue;			// continue the search from begining as this buf can not be used
				}
				else	// Not marked as delayed write
				{
					// Lock the buffer as it is now under use
					pAllotedBuff->ui16BufStatus = pAllotedBuff->ui16BufStatus | (LOCKED);
					// Remove the buf from the old(present) hash que	
					ui8RemoveBufferFromLinkList(HASH_QUE, pAllotedBuff);
					// Initialize the ui16BlockNo of that buffer with requested bluck no - KAPALAVARCHE PUSUN NAVIN LIHINE
					pAllotedBuff->ui16BlockNo = ui16RequestedBlockNo;
					// Mark the buffer as invaild as it is taken from free list and relocated on hash que
					pAllotedBuff->ui16BufStatus = pAllotedBuff->ui16BufStatus & (INVALID);
					// Add the buff to the new hash que as per the received block no
					ui8AddBufferToLinkList(&HashQueHeader[ui16RequestedBlockNo / MAX_NO_OF_BUF_PER_HASH_QUE], pAllotedBuff, BEGINING);
					//Break the infinite while loop as buf is acquired
					break;
				}
			}
		}	
	}
	// Fill the out parameter for the calling function with the address of acquired buffer
	*ppReturnBuf = pAllotedBuff;
	// buff successfully acquired from Hash Que
	return 0; 					
}




/*
This function receives the pointer to the buffer which is needed to be released
The function release the buffer and add it to the free list depending on its status
*/
UI8 ui8brelse(BUF_STRUCT* pBufToBeReleased)
{
	// If buffer is locked then only release it
	if (pBufToBeReleased->ui16BufStatus & LOCKED)
	{
		// Wake up all process waiting for any buffer to become free    // TRANSMIT SIGNAL @@@ TO GetBlk()
		// Wake up all process waiting for this buffer to become free	// TRANSMIT SIGNAL ### TO GetBlk()
		// raise the processor execution level

		// Buffer is valid and buff is not marked as old
		if ((pBufToBeReleased->ui16BufStatus & VALID) && !(pBufToBeReleased->ui16BufStatus & OLD))
		{
			// Add the buff at the end of the free list
			ui8AddBufferToLinkList(&FreeLstHeader, pBufToBeReleased, END);
#ifdef DEBUG
			printf("\nBuffer added at the END of free list");
#endif
		}
		else
		{
			// Add the buff at the begining of the free list
			ui8AddBufferToLinkList(&FreeLstHeader, pBufToBeReleased, BEGINING);
#ifdef DEBUG
			printf("\nBuffer added at the BEGINING of free list");
#endif
		}

		// lower the processor execution level	
		pBufToBeReleased->ui16BufStatus = pBufToBeReleased->ui16BufStatus & UNLOCK;  // Mark the buffer as free
	}
	else
	{
#ifdef DEBUG
		printf("\nBuffer is already on free list");
#endif
	}

	return 0;
}


/*
This function receives the block no which is needed to be read.
the function returns the buffer pointer which is allocated for
the requested block no. The return value is out parameter (RIKAME BHANDE)
The structure pointed by the return pointer always contain valid data. 
*/
UI8 ui8bread(UI16 ui16InputBlockNo, BUF_STRUCT** ppReturnBuf)
{
	BUF_STRUCT* pAllotedValidBuff = NULL;		//Local structure pointer 

	ui8GetBlk(ui16InputBlockNo, &pAllotedValidBuff);

	// Check that received buffer is valid or not. And read disk block if not valid
	if (!(pAllotedValidBuff->ui16BufStatus & VALID))
	{
		ui8ReadBlockFromHardDiskFile(pAllotedValidBuff);
	}

	// Fill the out parameter for the calling function with the address of acquired buffer
	*ppReturnBuf = pAllotedValidBuff;

	return 0;
}


/*
This function receives the pointer to the buffer which is needed to be write on the disk
It is always a valid buffer as it is going to be written on to the disk
*/
UI8 ui8bwrite(BUF_STRUCT* pBufToBeWritten)
{
	ui8WriteBlockToHardDiskFile(pBufToBeWritten);
	ui8brelse(pBufToBeWritten);

	return 0;
}


/*
This function receives the block number, it checks buffer pool and 
find outs the structure whcih is haveing the received block no.
the function returns the buffer pointer of the buffer having same block no.
The return value is out parameter (RIKAME BHANDE) ie parameterized return value
The function in short carries out the hash que search. the return pointer contains
NULL if the buffer with received block no is not found. ie buf not present on hash que.
*/
UI8 ui8BlockNoToBUffer(UI16 ui16InputBlockNo, BUF_STRUCT** ppReturnBuf)
{
	UI16 ui16LoopCounter;
	void* pLocalBuf;

	// Find the hash que for the requested blcok
	//ui16InputBlockNo / MAX_NO_OF_BUF_PER_HASH_QUE is that hash que 		
	// Assign first node on that hash que to satrt with 
	pLocalBuf = HashQueHeader[ui16InputBlockNo / MAX_NO_OF_BUF_PER_HASH_QUE].LinkList[HASH_QUE].pNextNode;
	pLocalBuf = ((GENERIC_NODE_STRUCT*)pLocalBuf)->pSelfAddr;
	// Header ui16BlockNo contains the present count of nodes on that link list
	ui16LoopCounter = HashQueHeader[ui16InputBlockNo / MAX_NO_OF_BUF_PER_HASH_QUE].ui16BlockNo;

	while (ui16LoopCounter)			// Search till whole linklist is not searched
	{
		// Is the current nodes block no matches with the block no which is received
		if (((BUF_STRUCT*)pLocalBuf)->ui16BlockNo == ui16InputBlockNo)
		{
			break;		// break the loop as the node is found
		}
		else
		{
			// Go to the next node as the current nodes block no does not matched with block no requested
			pLocalBuf = ((BUF_STRUCT*)pLocalBuf)->LinkList[HASH_QUE].pNextNode;
			pLocalBuf = ((GENERIC_NODE_STRUCT*)pLocalBuf)->pSelfAddr;;
			ui16LoopCounter--; // decrement the loopcounter to exit loop at end of link list
		}
	}
	// As loop counter is zero indicates required buff not found on hash que 
	// and control comes out of while due to end of link list
	if (ui16LoopCounter == 0) 		
	{
		pLocalBuf = NULL; // Assign NULL to the pointer which is to be returned as buf not found
	}
	
	*ppReturnBuf = pLocalBuf; //Fill the out parameter
	return 0;

}




/*
This function adds the node to the link list. The fucntion receives the 
HEAD of the link list to which the node is needed to be added. 
The function is receives the pointer of the buffer which is to be added.
The function receives the location in the link list where the received 
buffer is need to be added. For Hash que the buffer added at the begining.
For the free list the buffer is added at the begining as well as at the end.
*/
UI8 ui8AddBufferToLinkList(BUF_STRUCT* pHead, BUF_STRUCT* pBufToBeAdded, UI8 ui8Location)
{
	// As free list pointer is not NULL, this indicates that it is free list header 
	//and the buffer is to be added on Free List
	if (pHead->LinkList[FREE_LIST].pNextNode != NULL)	
	{
		if (ui8Location == BEGINING)
		{			
			ui8InsertNodeToLinkList((void*) pBufToBeAdded, (void*) pHead, TYPE_BUF_STRUCT, FREE_LIST);
		}
		else if (ui8Location == END)
		{
			ui8InsertNodeToLinkList((void*)pBufToBeAdded, (void*)((GENERIC_NODE_STRUCT*)(pHead->LinkList[FREE_LIST].pPrevNode))->pSelfAddr, TYPE_BUF_STRUCT, FREE_LIST);
		}
	}
	// As free list pointer is NULL, this indicates that it is hash que header 
	//and the buffer is to be added on Hash Que, add at the begining
	else 
	{
		ui8InsertNodeToLinkList((void*)pBufToBeAdded, (void*)pHead, TYPE_BUF_STRUCT, HASH_QUE);
	}
	// As the new node is added on link list increment the count of nodes on link list
	// Header ui16BlockNo contains the present count of nodes on that link list
	pHead->ui16BlockNo = pHead->ui16BlockNo + 1;			
//	pHead->ui16BufStatus = NULL;						// Can be used for additional info
//	pHead->pBufIncoreDiskData = NULL;					// can be used for additional info

	return 0;
}




/*
This function removes the node from the link list. The fucntion receives the
type of the link list from which the node is needed to be added.
The function is receives the pointer of the buffer which is to be removed.
*/
UI8 ui8RemoveBufferFromLinkList(UI8 ui8LinkList, BUF_STRUCT* pRemovedBuff)
{
	if (ui8LinkList == FREE_LIST)
	{

		ui8RemoveNodeFromLinkList((void*)pRemovedBuff, TYPE_BUF_STRUCT, FREE_LIST);

		// As the new node is removed from link list decrement the count of nodes on link list
		// Header ui16BlockNo contains the present count of nodes on that link list
		FreeLstHeader.ui16BlockNo = FreeLstHeader.ui16BlockNo - 1;			
	}
	else if (ui8LinkList == HASH_QUE)
	{
		ui8RemoveNodeFromLinkList((void*)pRemovedBuff, TYPE_BUF_STRUCT, HASH_QUE);

		// As the new node is removed from link list decrement the count of nodes on link list
		// Header ui16BlockNo contains the present count of nodes on that link list
		// Find the hash que for the requested blcok
		// pRemovedBuff->ui16BlockNo / MAX_NO_OF_BUF_PER_HASH_QUE gives the hash que haeder
		HashQueHeader[pRemovedBuff->ui16BlockNo / MAX_NO_OF_BUF_PER_HASH_QUE].ui16BlockNo = HashQueHeader[pRemovedBuff->ui16BlockNo / MAX_NO_OF_BUF_PER_HASH_QUE].ui16BlockNo - 1;
	}
	return 0;
}




/*
This function removes the node from the link list. The fucntion receives the
type of the link list from which the node is needed to be added.
The function is receives the pointer of the buffer which is to be removed.
*/
UI8 ui8InitBufPool(void)
{
	UI16 ui16LoopCounter;

	for (ui16LoopCounter = 0; ui16LoopCounter < MAX_NO_OF_BUF; ui16LoopCounter++)
	{
		Buf[ui16LoopCounter].ui16BlockNo = ui16LoopCounter;
		Buf[ui16LoopCounter].ui16BufStatus &= (ALL_CLR_UI16 & INVALID & UNLOCK);
		Buf[ui16LoopCounter].pBufIncoreDiskData = &ui8BufIncoreDiskBlockData[ui16LoopCounter];

		ui8InitLinkListNode((void*)&Buf[ui16LoopCounter], TYPE_BUF_STRUCT, FREE_LIST);
		ui8InitLinkListNode((void*)&Buf[ui16LoopCounter], TYPE_BUF_STRUCT, HASH_QUE);

	}


	for (ui16LoopCounter = 0; ui16LoopCounter < NO_OF_HASH_QUE; ui16LoopCounter++)
	{
		HashQueHeader[ui16LoopCounter].ui16BlockNo = V_NULL;
		HashQueHeader[ui16LoopCounter].ui16BufStatus = ALL_SET_UI16;
		HashQueHeader[ui16LoopCounter].pBufIncoreDiskData = P_NULL;

		ui8InitLinkListNode((void*)&HashQueHeader[ui16LoopCounter], TYPE_BUF_STRUCT, HASH_QUE);
		ui8ClearLinkListNode((void*)&HashQueHeader[ui16LoopCounter], TYPE_BUF_STRUCT, FREE_LIST);

		ui8InitLinkListHeader((void*)&HashQueHeader[ui16LoopCounter], TYPE_BUF_STRUCT, HASH_QUE);
		ui8ClearLinkListHeader((void*)&HashQueHeader[ui16LoopCounter], TYPE_BUF_STRUCT, FREE_LIST);

	}

	{
		FreeLstHeader.ui16BlockNo = V_NULL;
		FreeLstHeader.ui16BufStatus = ALL_SET_UI16;
		FreeLstHeader.pBufIncoreDiskData = P_NULL;

		ui8InitLinkListNode((void*)&FreeLstHeader, TYPE_BUF_STRUCT, FREE_LIST);
		ui8ClearLinkListNode((void*)&FreeLstHeader, TYPE_BUF_STRUCT, HASH_QUE);

		ui8InitLinkListHeader((void*)&FreeLstHeader, TYPE_BUF_STRUCT, FREE_LIST);
		ui8ClearLinkListHeader((void*)&FreeLstHeader, TYPE_BUF_STRUCT, HASH_QUE);
	}

	for (ui16LoopCounter = 0; ui16LoopCounter < MAX_NO_OF_BUF; ui16LoopCounter++)
	{
		ui8AddBufferToLinkList((BUF_STRUCT*)&FreeLstHeader, (BUF_STRUCT*)&Buf[ui16LoopCounter], END);
	}

	for (ui16LoopCounter = MAX_NO_OF_BUF; ui16LoopCounter > 0; ui16LoopCounter--)
	{
		ui8AddBufferToLinkList((BUF_STRUCT*)&HashQueHeader[0], (BUF_STRUCT*)&Buf[ui16LoopCounter - 1], BEGINING);
	}

	return 0;
}
