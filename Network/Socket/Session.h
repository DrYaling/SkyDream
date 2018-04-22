#ifndef _SESSION_H
#define _SESSION_H
class Session 
{
public:
	void ResetTimeOutTime();
	void QueuePacket(char* pack);
};
#endif