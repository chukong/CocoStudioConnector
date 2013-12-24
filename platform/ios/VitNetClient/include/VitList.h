
#ifndef _VITLIST_H_LEE_
#define _VITLIST_H_LEE_

#include "VitPcheader.h"
#include "VitConnectDefine.h"
#include "VitMsgSn.h"
#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#include <WinSock2.h>
#endif

typedef struct t_SListNode
{
	protocol_t             id;		//--protocol id--
	Connect_RecvMsg_cb	   cb;		//--callback pointer--
	struct t_SListNode*    next;	//--next node--
}SListNode;
/*
*	function  : create ListNode object and Init all parameter
*	parameter : void
*	return    : SListNode Pointer
*/
extern SListNode* List_CreateHead();
/*
*	function  : to add new node in the list
*	parameter : head : listnode point,id:protocol id,cb:callback
*	return    : void
*/
extern void List_Add( SListNode* head,protocol_t id,Connect_RecvMsg_cb cb );
/*
*	function  : from head find the corresponding callback
*	parameter : head : listnode point,id:protocol id
*	return    : SListNode Pointer
*/
extern SListNode*  List_Find( SListNode* head,protocol_t id );
/*
*	function  : from head del the corresponding callback
*	parameter : head : listnode point,id:protocol id
*	return    : true del ok
*/
extern bool_t List_Delete( SListNode* head,protocol_t id );
/*
*	function  : free list obj
*	parameter : head : listnode point,id:protocol id
*	return    : void
*/
extern void List_Free( SListNode* head );




typedef struct t_SThreadListNode
{
    bool_t     bState;
#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
	HANDLE	                    insThreadID;        //--thread handle--
#else
	pthread_t                   insThreadID;        //--thread handle--
#endif
	struct t_SThreadListNode*    next;	//--next node--
}SThreadListNode;

/*
 *	function  : create ListNode object and Init all parameter
 *	parameter : void
 *	return    : SListNode Pointer
 */
extern SThreadListNode* ThreadList_CreateHead();
/*
 *	function  : to add new node in the list
 *	parameter : head : listnode point,id:protocol id,cb:callback
 *	return    : void
 */
extern void ThreadList_Add( SThreadListNode* head,protocol_t id,Connect_RecvMsg_cb cb );
/*
 *	function  : from head find the corresponding callback
 *	parameter : head : listnode point,id:protocol id
 *	return    : SListNode Pointer
 */
extern SListNode*  ThreadList_Find( SThreadListNode* head,protocol_t id );
/*
 *	function  : from head del the corresponding callback
 *	parameter : head : listnode point,id:protocol id
 *	return    : true del ok
 */
extern bool_t ThreadList_Delete( SThreadListNode* head,protocol_t id );
/*
 *	function  : free list obj
 *	parameter : head : listnode point,id:protocol id
 *	return    : void
 */
extern void ThreadList_Free( SThreadListNode* head );

#endif