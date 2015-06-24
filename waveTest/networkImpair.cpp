#include "structIni.h"
#include <sstream>

void networkImpairment::countIni()
{
	this->total_loss_num = 0;
	this->total_packet_num = 0;
}
std::string networkImpairment::toString()
{
	std::stringstream ostream;
	std::string impair_content;
	ostream<<this->total_loss_num<<","<<this->total_packet_num;
	ostream>>impair_content;
	return impair_content;
}
std::string networkImpairment::getFrameNumPerPacket()
{
	float loss_rate = this->total_loss_num/this->total_packet_num;
	int frames = 1;
	std::stringstream ostream;
	std::string frames_per_packet;
	
	if(loss_rate<0.05)
	{
		frames = 1;
	}
	else if(loss_rate<0.1 || loss_rate>0.05)
	{
		frames = 2;
	}
	else
	{
		frames = 4;
	}
	ostream<<frames;
	ostream>>frames_per_packet;

	return frames_per_packet;
}
void networkImpairment::addLossNum(int loss_num)
{
	this->total_loss_num += loss_num;
	this->total_packet_num += loss_num+1;
}
int networkImpairment::getInterval()
{
	return this->interval_every_send;
}