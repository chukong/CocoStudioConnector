#include "VitConnectDefine.h"
#include "VitPcheader.h"
#include "VitBuffer.h"
#include "VitLog.h"


SVitBuffer*	Buffer_New( int maxsize )
{
	SVitBuffer* p = (SVitBuffer*)malloc(sizeof(SVitBuffer));
	if( !p )
	{
		VITLOG(1,"(%s %d %s) Memory malloc is failed", __FILE__,__LINE__,__FUNCTION__); return NULL;
	}
	p->pBuffer = (char*)malloc(maxsize);
	p->nMaxSize = maxsize;

	if( !p->pBuffer )
	{
		VITLOG(1,"(%s %d %s) Memory malloc is failed",__FILE__,__LINE__,__FUNCTION__); return NULL;
	}
	Buffer_Clear(p);
	return p;
}

void	Buffer_Free( SVitBuffer* p )
{
	if( p == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter p is null", __FILE__,__LINE__,__FUNCTION__); return;
	}
	free(p->pBuffer);
	p->pBuffer = NULL;
	free(p);
	p = NULL;
}

void	Buffer_Clear( SVitBuffer* p )
{
	if( p == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter p is null", __FILE__,__LINE__,__FUNCTION__); return;
	}

	p->nTail = p->nHead = 0;
}
void Buffer_SetHead( SVitBuffer* p,int head )
{
	if( p == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter p is null", __FILE__,__LINE__,__FUNCTION__); return;
	}
	p->nHead = head;
}

bool_t	Buffer_WriteToBuff( SVitBuffer* p,const void* buf, int len )
{
	if( p == NULL || buf == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter p || buf is null", __FILE__,__LINE__,__FUNCTION__); return 0;
	}

	if( !Buffer_CheckFull(p,len) )
		return 0;
	
	memcpy(p->pBuffer+p->nTail, buf, len) ;
	p->nTail += len ;
	return 1 ;
}

bool_t	Buffer_ReadFromBuff( SVitBuffer* p,void* out_buf, int len )
{
	if( p == NULL || out_buf == NULL )
	{
		VITLOG(1,"(%s %d %s) parameter p is null", __FILE__,__LINE__,__FUNCTION__); return 0;
	}

	//-When Buffer is Empty-
	if( !Buffer_CheckEmpty(p,len) )
	{
		Buffer_Clear(p);
		return 0;
	}
	memcpy(out_buf, p->pBuffer+p->nHead, len) ;
	p->nHead += len ;
	if( p->nHead == p->nTail )
	{
		Buffer_Clear(p);
	}
	return 1 ;
}

bool_t	Buffer_CheckEmpty( SVitBuffer* p,int len )
{
	if( !p )
	{
		VITLOG(1,"(%s %d %s) parameter p is null",__FILE__,__LINE__,__FUNCTION__); return 0;
	}

	if( (p->nHead + len) > p->nTail )
	{
		VITLOG(1,"(%s %d %s) Cur buffer bis full taile=[%d],head=[%d],max=[%d],len=[%d]",__FILE__,__LINE__,__FUNCTION__,p->nTail,p->nHead,p->nMaxSize,len ); return 0;
	}
	else if( p->nHead == p->nTail )
	{
		VITLOG(1,"(%s %d %s) Cur buffer bis empty"); return 0;
	}
	return 1;
}

bool_t    Buffer_CheckFull( SVitBuffer* p,int len )
{
	if( !p )
	{
		VITLOG(1,"(%s %d %s) parameter p is null",__FILE__,__LINE__,__FUNCTION__); return 0;
	}
	if( p->nHead == p->nTail )
	{
		Buffer_Clear(p);
        return 1;
	}
	if( (p->nTail + len) > p->nMaxSize )
	{
		int curlen = p->nTail - p->nHead;
		memmove( p->pBuffer, p->pBuffer+p->nHead,curlen );
		p->nHead = 0;
		p->nTail = curlen;
		if( curlen + len < p->nMaxSize )
			return 1;
		VITLOG(1,"(%s %d %s) Cur buffer is full nTail=[%d],head=[%d],max=[%d],len=[%d]",__FILE__,__LINE__,__FUNCTION__,p->nTail,p->nHead,p->nMaxSize,len ); return 0;
	}
	else if( p->nTail >= p->nMaxSize )
	{
		VITLOG(1,"(%s %d %s) Cur buffer is full nTail=[%d],head=[%d],max=[%d],len=[%d]",__FILE__,__LINE__,__FUNCTION__,p->nTail,p->nHead,p->nMaxSize,len ); return 0;
	}

	return 1;
}

bool_t	Buffer_IsEmpty( SVitBuffer* p )
{
	if( p && p->nTail == p->nHead )
		return 1;
	return 0;
}

int		Buffer_GetCurLen( SVitBuffer* p )
{
	if( !p )
	{
		VITLOG(1,"(%s %d %s) parameter p is null",__FILE__,__LINE__,__FUNCTION__); return 0;
	}
	return p->nTail - p->nHead;
}

bool_t Buffer_GetMsg( SVitBuffer* p,void* out_buf,int* out_size )
{
	if( !p )
	{
		VITLOG(1,"(%s %d %s) parameter p is null",__FILE__,__LINE__,__FUNCTION__); return 0;
	}
	bool_t bS = 0;
	int strlen = 0;
	for( int i = p->nHead; i < p->nTail; ++i)
	{
		if( bS )
		{	
			if( p->pBuffer[i] == '#' )
			{
				if( p->pBuffer[++i] == '@' )
				{
					strlen = i - p->nHead - 2- 1;
					memcpy( out_buf,p->pBuffer+p->nHead+2,strlen );
					*out_size = strlen ;
					Buffer_SetHead( p,i+1 );
					if( p->nHead == p->nTail )
						Buffer_Clear(p);
					return 1;
				}
			}
			else if( p->pBuffer[i] == '*' )
			{
				if( p->pBuffer[++i] == '^' )
				{
					Buffer_SetHead(p,i-1);
					return Buffer_GetMsg( p,out_buf,out_size );
				}
			}
		}
		else
		{
			if( p->pBuffer[i] == '*' )
			{
				if( p->pBuffer[++i] == '^' )
				{
					bS = 1;
				}
			}
			else if( p->pBuffer[i] == '#' )
			{
				if( p->pBuffer[++i] == '@' )
				{
					Buffer_SetHead(p,i+1);
					bS = 0;
					return Buffer_GetMsg( p,out_buf,out_size );
				}		
			}
		}
	}
	return 0;
}