/*
* Copyright (c) 2012 Chukong Technologies, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
* NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
* USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*  FileName : Protocol.h
*  Use      : protocol
*  Version  : Base 1.0.0
*  Author   : Lee
*  Date     : 2012-01-06
*  Note     :
*/


#ifndef _VIT_PROTOCOL_LEE_
#define _VIT_PROTOCOL_LEE_

//--注意: 应用层里协议ID 需要大与1000--

enum eVitProtocol
{
	eProtocol_InlineStart = 0,
	/************************************************************************/
	/*                      LoginServer => GateServer                       */
	/************************************************************************/
	/* parameter: SHeader_ID_LEN_SIGN,void* client packet */
	eLG_PassClientPackToLoginReq,                           //--从客户端收到的信息包发送到login--
	eLG_PassLoginPackToClientAck,                           //--从login服务器向客户端发送信息--
	/* parameter: SHeader_ID_LEN_SIGN */
	eLG_AcceptNewClientReq,                                 //--客户端上线Gateserver向Loginserver发送信息--
	eLG_DisconnectClientReq,                                //--客户端下线GateServer向Loginserver发送信息--		
	/* parameter: SHeader_ID_LEN_SIGN ,int nCurUserNum */
	eLG_LoginUserNumAck,                                    //--当前在login服务器里的玩家个数包--
	/************************************************************************/
	/*                      GateServer => GameServer                        */
	/************************************************************************/
	/* parameter: SHeader_ID_LEN_SIGN,void* client packet */
	eGG_PassClientPackToGameReq,                            //--客户端的packet直接向游戏服务器传送协议--
	eGG_PassGamePackToClientAck,                            //--游戏服务器的packet直接向客户端传送协议--
	/* parameter: SHeader_ID_LEN_SIGN */
	eGG_AcceptNewClientReq,                                 //--客户端上限的时候Gate向Game 发的信息--
	eGG_DisconnectClientReq,                                //--客户端下线的时候Gate向Game 发的信息--
	/* parameter: SHeader_ID_LEN_SIGN ,int nCurUserNum */
	eGG_GameUserNumAck,                                     //--当前在GameServer里的玩家个数包--
	/************************************************************************/
	/*                      GateServer => Client                            */
	/************************************************************************/
	/* parameter: SHeader_UDP_ID_LEN */
	eGTC_UdpConnectReq,                                     //--向服务器请求UDP连接--
	eGTC_UdpConnectAck,                                     //--UDP连接成功返回包--
	/* parameter: SHeader_UDP_ID_LEN */
	eGTC_UdpOnLineStateReq,                                 //--客户端向服务器发送在线确认包--
	eGTC_UdpOnLineStateAck,                                 //--一定时间内服务器没有收到包 发送此包到--	
	eGTC_TcpDisconnectAck,                                  //--与服务器断开连接--
	eProtocol_InlineEnd = 999,
//********************************************************************************************
};


#endif 