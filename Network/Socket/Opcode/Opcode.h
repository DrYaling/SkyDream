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
	S2C_PUNCH = 301,//֪ͨ�ͻ��˽��д�
	S2C_PUNCH_COMFIRM = 302,//֪ͨ�ͻ��˽��д�
	S2C_CLIST = 2,//��ȡ�ͻ����б�
	S2C_CLIENT_ID  = 3,//֪ͨ�ͻ���id��

};
enum C2S_Opcode 
{
	C2S_PUNCH =300,
	C2S_CLIST = 2,//����ͻ����б�
};
enum C2C_Opcode
{
	C2C_HAND = 1,//�ͻ�������
	C2C_PING = 2,//�ͻ�������
	C2C_MSG = 3,//�ͻ����ı���Ϣ
};
#endif