#include <assert.h>
#include "list.h"
#define NULL 0
static Node allNodes[LIST_MAX_NUM_NODES];
static List allListHeads[LIST_MAX_NUM_HEADS];

static Node* freeNode = NULL;
static Node* freeHead = NULL;
static bool firstCall = true;


//redo base newNode logic to be consective nodes
//base -> newNode
static void connectNodes(Node* base,Node* newNode){
    assert(base != NULL);
    newNode->prev = base;
    newNode->next = base->next;
    base->next = newNode;
    newNode->next->prev = newNode;
    
    
}
static void initializeEmptyList(List* pList,Node* newNode){
    pList->pHead = newNode;
    pList->pHead->next = pList->pHead;
    pList->pHead->prev = pList->pHead;
    pList->numElements++;
    pList->pCurrentItem = newNode;
}
//must redo using list method
static Node* createGenericNode(void* pItem){
    if(freeNode == NULL){
        assert(true);
        return NULL;
        
    }
    Node* newNode = freeNode;
    freeNode = freeNode->next;
    newNode->pItem = pItem;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}
static void returnNode(Node* NodeToReturn){
    NodeToReturn->prev = NULL;
    NodeToReturn->pItem = NULL;
    NodeToReturn->next = freeNode;
    freeNode = NodeToReturn;
}
//List* must be empty!
static void returnHead(List* headToReturn){
    assert(headToReturn->numElements == 0);
    assert(headToReturn->pHead == NULL);
    assert(headToReturn->pCurrentItem == NULL);
    assert(freeNode != NULL);
    Node* newNode = createGenericNode(headToReturn);
    newNode->next = freeHead;
    freeHead = newNode;

}
// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
//O(N) on first run
List* List_create(){
    if(firstCall){
        firstCall = false;
        for(int i = 0; i < LIST_MAX_NUM_NODES -1;i++){
            allNodes[i].next = &allNodes[i+1];
            allNodes[i].pItem = NULL;
            allNodes[i].prev = NULL;
        }
        allNodes[LIST_MAX_NUM_NODES-1].pItem = NULL;
        allNodes[LIST_MAX_NUM_NODES-1].prev = NULL;
        freeNode = &allNodes[0];
        
        freeHead = createGenericNode(&allListHeads[0]);
        Node* newNode = NULL;
        assert(LIST_MAX_NUM_HEADS < LIST_MAX_NUM_NODES);
        for(int j = 1; j < LIST_MAX_NUM_HEADS; j++){
            newNode = createGenericNode(&allListHeads[j]);
            newNode->next = freeHead;
            freeHead = newNode;
        }
        
    }
    Node* newNode = freeHead;
    freeHead = freeHead->next;
    newNode->next = NULL;
    List* newList = newNode->pItem;
    returnNode(newNode);
    newList->numElements =0;
    newList->pCurrentItem = NULL;
    newList->pHead =NULL;
    newList->state  = LIST_OOB_START;
    return newList;  
}

// Returns the number of items in pList.
int List_count(List* pList){
    assert(pList != NULL);
    return (*pList).numElements;

}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList){
    assert(pList->numElements >= 0);
    if(pList->numElements == 0){
        pList->pCurrentItem = NULL;
        return NULL;
    }
    pList->pCurrentItem =  pList->pHead;
    pList->state = LIST_OOB_START;
    return pList->pCurrentItem->pItem;

}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList){
    assert(pList->numElements >= 0);
    if(pList->numElements == 0){
        pList->pCurrentItem = NULL;
        return NULL;
    }
    pList->pCurrentItem =  pList->pHead->prev;
    pList->state = LIST_OOB_END;
    return pList->pCurrentItem->pItem;
} 

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList){
   assert(pList != NULL);
    if(pList->numElements == 0 || (pList->pCurrentItem == NULL && pList->state == LIST_OOB_END)){
        pList->pCurrentItem = NULL;
        pList->state = LIST_OOB_END;
        return NULL;
    }else if(pList->pCurrentItem == NULL && pList->state == LIST_OOB_START){
        pList->pCurrentItem = pList->pHead;
        return pList->pCurrentItem->pItem;
    }else if (pList->pCurrentItem->next == pList->pHead){
        pList->pCurrentItem = NULL;
        pList->state = LIST_OOB_END;
        return NULL;
    }else{
        pList->pCurrentItem = pList->pCurrentItem->next;
        return pList->pCurrentItem->pItem;
    }
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList){
    assert(pList != NULL);
    if(pList->numElements == 0 || (pList->pCurrentItem == NULL && pList->state == LIST_OOB_START)){
        pList->pCurrentItem = NULL;
        pList->state = LIST_OOB_START;
        return NULL;
    }else if(pList->pCurrentItem == NULL && pList->state == LIST_OOB_END){
        pList->pCurrentItem = pList->pHead->prev;
        return pList->pCurrentItem->pItem;
    }else if (pList->pCurrentItem->prev == pList->pHead->prev){
        pList->pCurrentItem = NULL;
        pList->state = LIST_OOB_START;
        return NULL;
    }else{
        pList->pCurrentItem = pList->pCurrentItem->prev;
        return pList->pCurrentItem->pItem;
    }
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList){
    assert(pList->pCurrentItem != NULL && "Current is NULL");
    return pList->pCurrentItem->pItem;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem){
    assert(pList != NULL);
    if(freeNode != NULL){
        //List is empty
        Node* newNode = createGenericNode(pItem);
        if(pList->numElements == 0){
            initializeEmptyList(pList,newNode);
            pList->pCurrentItem = pList->pHead;
        }else{
            if( pList->pCurrentItem == NULL){
                if(pList->state == LIST_OOB_START){
                    connectNodes(pList->pHead->prev,newNode);
                    pList->pHead = pList->pHead->prev;
                    pList->pCurrentItem = pList->pHead;
                    pList->numElements++;
                }else{
                    connectNodes(pList->pHead->prev,newNode);
                    pList->pCurrentItem = pList->pHead->prev;
                    pList->numElements++;
                }
            }else{
                connectNodes(pList->pCurrentItem,newNode);
                pList->pCurrentItem = pList->pCurrentItem->next;
                pList->numElements++;    
            }
        }
        return 0;
    }else{
        return -1;
    }
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem){
    assert(pList != NULL);
    if(freeNode != NULL){
        //List is empty
        Node* newNode = createGenericNode(pItem);
        if(pList->numElements == 0){
            initializeEmptyList(pList,newNode);
            pList->pCurrentItem = pList->pHead;
        }else{
            if( pList->pCurrentItem == NULL){
                if(pList->state == LIST_OOB_START){
                    connectNodes(pList->pHead->prev,newNode);
                    pList->pHead = pList->pHead->prev;
                    pList->pCurrentItem = pList->pHead;
                    pList->numElements++;
                }else{
                    connectNodes(pList->pHead->prev,newNode);
                    pList->pCurrentItem = pList->pHead->prev;
                    pList->numElements++;
                }
            }else{
                connectNodes(pList->pCurrentItem->prev,newNode);
                if(pList->pCurrentItem == pList->pHead){
                    pList->pHead = pList->pHead->prev;
                }
                pList->pCurrentItem = pList->pCurrentItem->prev;
                pList->numElements++;    
                
            }
        }
        return 0;
    }else{
        return -1;
    }
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
//redo
int List_append(List* pList, void* pItem){
    assert(pList != NULL);
    if(freeNode != NULL){
        //List is empty
        Node* newNode = createGenericNode(pItem);
        if(pList->numElements == 0){
            initializeEmptyList(pList,newNode);
            pList->pCurrentItem = pList->pHead;
        }else{
            connectNodes(pList->pHead->prev,newNode);
            pList->pCurrentItem = pList->pHead->prev;
            pList->numElements++;
        }
        return 0;
    }else{
        return -1;
    }
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem){
    assert(pList != NULL);
    if(freeNode != NULL){
        //List is empty
        Node* newNode = createGenericNode(pItem);
        if(pList->numElements == 0){
            initializeEmptyList(pList,newNode);
            pList->pCurrentItem = pList->pHead;
        }else{
            connectNodes(pList->pHead->prev,newNode);
            pList->pCurrentItem = pList->pHead->prev;
            pList->numElements++;
            pList->pHead = pList->pHead->prev;
        }
        return 0;
    }else{
        return -1;
    }
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList){
    if(pList->pCurrentItem == NULL){
        return NULL;
    }else if(pList->numElements == 1){
        pList->numElements--;
        void* pItem = pList->pCurrentItem->pItem;
        returnNode(pList->pCurrentItem);
        pList->pCurrentItem = NULL;
        pList->pHead = NULL;
        pList->state = LIST_OOB_START;
        return pItem;
    }else{
        Node* PrevNode = pList->pCurrentItem->prev;
        Node* NextNode = pList->pCurrentItem->next;
        NextNode->prev = PrevNode;
        PrevNode->next = NextNode;
        pList->numElements--;
        void* pItem = pList->pCurrentItem->pItem;
        if(pList->pHead == pList->pCurrentItem){
            pList->pHead = NextNode;
        }
        returnNode(pList->pCurrentItem);
        pList->pCurrentItem = NextNode;
        return pItem;
    }
    
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList){
     if(pList->pHead == NULL){
        return NULL;
    }else if(pList->numElements == 1){
        pList->numElements--;
        void* pItem = pList->pHead->pItem;
        returnNode(pList->pHead);
        pList->pCurrentItem = NULL;
        pList->pHead = NULL;
        pList->state = LIST_OOB_START;
        return pItem;
    }else{
        Node* NodeToDelete = pList->pHead->prev;
        Node* PrevNode = pList->pHead->prev->prev;
        Node* NextNode = pList->pHead;
        NextNode->prev = PrevNode;
        PrevNode->next = NextNode;
        pList->numElements--;
        void* pItem = NodeToDelete->pItem;
        returnNode(NodeToDelete);
        return pItem;
    }
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2){
    if(pList1->numElements == 0 || pList2->numElements == 0){
        if (pList1->numElements == 0){
            List* temp;
            temp = pList1;
            pList1 = pList2;
            pList2 = temp;
        }
        returnHead(pList2);
    }else{
        Node* pBaseHead = pList1->pHead;
        Node* pBaseTail = pList1->pHead->prev;
        Node* pNewMid = pList2->pHead;
        Node* pNewTail = pList2->pHead->prev;
        pList1->numElements = pList1->numElements + pList2->numElements;
        pList2->numElements = 0;
        pList2->pCurrentItem = NULL;
        pList2->pHead = NULL;
        returnHead(pList2);
        pBaseHead->prev = pNewTail;
        pNewTail->next = pBaseHead;
        pBaseTail->next = pNewMid;
        pNewMid->prev = pBaseTail;
    }

}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn){
    while(pList->numElements != 0){
        pItemFreeFn(pList->pHead->prev);
        pList->numElements--;
    }
    if(pList->numElements == 0){
        returnHead(pList);
    }
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg){
    if(pList->pCurrentItem == NULL && pList->state == LIST_OOB_START){
        pList->pCurrentItem = pList->pHead;
    }
    while(pList->pCurrentItem != NULL){
        if(pComparator(pList->pCurrentItem->pItem,pComparisonArg)){
            return pList->pCurrentItem->pItem;
        }
        List_next(pList);
    }
    return NULL;

}