#include "rtpRecv.h"

RtpSession *session;
//const int port = 8000;
//const int jittcomp=40;
//const bool adapt=1;

//FILE *fp;
int rtpRecvInitalize(int port, bool adapt, int jittcomp)
{
	ortp_init();
	ortp_scheduler_init();
	ortp_set_log_level_mask(ORTP_DEBUG|ORTP_MESSAGE|ORTP_WARNING|ORTP_ERROR);
	session=rtp_session_new(RTP_SESSION_RECVONLY);	
	rtp_session_set_scheduling_mode(session,1);
	rtp_session_set_blocking_mode(session,1);
	rtp_session_set_local_addr(session,"0.0.0.0",port,-1);
	rtp_session_set_connected_mode(session,TRUE);
	rtp_session_set_symmetric_rtp(session,TRUE);
	rtp_session_enable_adaptive_jitter_compensation(session,adapt);
	rtp_session_set_jitter_compensation(session,jittcomp);
	rtp_session_set_payload_type(session,0);
	rtp_session_set_recv_buf_size(session, 400);
	rtp_session_signal_connect(session,"ssrc_changed",(RtpCallback)rtp_session_reset,0);
	return 1;
}
int rtpRecv(unsigned char* buffer,int &size)
{
	uint32_t ts=0;
	mblk_t *mp=NULL;
	int package_size = 0;
	while(1)
	{
		mp = rtp_session_recvm_with_ts(session,ts);
		if(mp)
		{
			package_size = rtp_get_payload(mp, &mp->b_rptr);
			buffer = (unsigned char*)realloc(buffer, size+package_size);			
			memcpy(buffer+size, mp->b_rptr, package_size);
			size += package_size;
		}
		ts += 160;
	}
	return 1;
}
DWORD WINAPI rtpRecv(LPVOID pParam)
{
	DoubleBuf *doubleBuf = (DoubleBuf*)pParam;
	CCircularBuf *buffer = &(doubleBuf->rtpBuf);
	networkImpairment net_impair = doubleBuf->net_impair;
	int package_size = 0;
	int count=0;
	net_impair.countIni();

	int prev_seq_num = -1,current_seq_num=-1;
	int loss_num = 0,i;
	uint32_t ts=0;
	mblk_t *mp=NULL;
	//
	//fp = fopen("..\\rtpIn.wbp","wb+");
	while(1)
	{
		mp = rtp_session_recvm_with_ts(session,ts);
		if(mp)
		{
			count++;
			//计算丢包数量
			current_seq_num = rtp_get_seqnumber(mp);
			if(prev_seq_num == -1)
				prev_seq_num = current_seq_num;
			else
				loss_num = current_seq_num-prev_seq_num-1;

			prev_seq_num = current_seq_num;
			net_impair.addLossNum(loss_num);

			package_size = rtp_get_payload(mp, &mp->b_rptr);
			//如果接受的是空包，退出线程
			if(package_size == 0)
				break;

			//if(WaitForSingleObject(doubleBuf->hRtpWrite, 20) == WAIT_OBJECT_0)
			//{
				buffer->writeBuf((const char*)mp->b_rptr, package_size);
				printf("receive %d package|current sequence number is %d\n",count,current_seq_num);
			/*}*/
			
			//fwrite(mp->b_rptr, package_size, 1, fp);			
		}
		ts += 160;
	}
	return 0;
}
int rtpDestory()
{
	//fclose(fp);
	rtp_session_destroy(session);
	ortp_exit();
	
	ortp_global_stats_display();
	return 1;
}