#ifndef __MAIN_H__
#define __MAIN_H__

#define PS2_FLAG_RCV_COMPLETE   (1<<0)
#define PS2_FLAG_RCV_RECEIVING  (1<<1)
#define PS2_FLAG_RCV_ERROR      (1<<2)

enum ps2ReceiveState {
	start   =   0,
	data	=	1,
	parity  =   9, 
	stop    =   10
};

#endif //__MAIN_H__
