#include"VitList.h"

SListNode* List_CreateHead()
{
	SListNode* p = (SListNode*)malloc(sizeof( SListNode ));
	if( p == NULL )
	{
		VITLOG(1,"(%s %d %s) New SListNode is Failed", __FILE__,__LINE__,__FUNCTION__); return NULL;
	}
	p->id = -1;
	p->next = NULL;
	return p;
}

void List_Add( SListNode* head,protocol_t id,Connect_RecvMsg_cb cb )
{
	if( head == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter head is null", __FILE__,__LINE__,__FUNCTION__); return;
	}
	SListNode* p = (SListNode*)malloc(sizeof( SListNode ));
	if( p == NULL )
	{
		VITLOG(1,"(%s %d %s) New SListNode is Failed", __FILE__,__LINE__,__FUNCTION__); return;
	}
	p->id = id;
	p->cb = cb;
	p->next = NULL;

	if( head == NULL )
	{
		head = p;
	}
	else
	{
		SListNode* next = head;
		while ( next )
		{
			if( next->next == NULL )
			{
				next->next = p;
				return;
			}
			next = next->next;
		}
		head->next = p;
	}
}

SListNode*  List_Find( SListNode* head,protocol_t id )
{
	if( head == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter head is null", __FILE__,__LINE__,__FUNCTION__); return NULL;
	}
	SListNode* p = head;
	while ( p )
	{
		if( p->id == id )
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

bool_t List_Delete( SListNode* head,protocol_t id )
{
	if( head == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter head is null", __FILE__,__LINE__,__FUNCTION__); return 0;
	}
	SListNode* p = head;
	if( head->id == id )
	{
		free(p);
		return 1;
	}

	while ( p )
	{
		SListNode* next = p->next;
		if( next == NULL)
		{
			if( p->id == id )
				return 1;
			else
				return 0;
		}

		if( next->id == id )
		{
			p->next = p->next->next;
			free(next);
			next = NULL;
			return 1;
		}
		p = p->next;
	}
	return 0;
}

void List_Free( SListNode* head )
{
	if(head != NULL)
	{
		free(head);
		head = NULL;	
	}
}






