#ifndef _OPCODE_H
#define _OPCODE_H
struct punch_content
{
public:
	int32 from_;
	int32 to_;
};
enum S2C_Opcode 
{
	S2C_PUNCH = 301,//通知客户端进行打洞
	S2C_PUNCH_COMFIRM = 302,//通知客户端进行打洞
	S2C_CLIST = 2,//获取客户端列表
	S2C_CLIENT_ID  = 3,//通知客户端id号

};
enum C2S_Opcode 
{
	C2S_PUNCH =300,
	C2S_CLIST = 2,//请求客户端列表
};
enum C2C_Opcode
{
	C2C_HAND = 1,//客户端握手
	C2C_PING = 2,//客户端心跳
	C2C_MSG = 3,//客户端文本消息
};
#endif