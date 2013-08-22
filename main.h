#ifndef __MAIN_H__
#define __MAIN_H__

#define PS2_RCV_FLAG_COMPLETE   (1<<0)
#define PS2_RCV_FLAG_RECEIVING  (1<<1)
#define PS2_RCV_FLAG_ERROR      (1<<2)

#define PS2_DATA        PD1
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_PORT   PORTD

#define PS2_CLK         PD2
#define PS2_CLK_PIN     PIND
#define PS2_CLK_DDR     DDRD
#define PS2_CLK_PORT    PORTD

enum ps2ReceiveState {
	start   =   0,
	data	=	1,
	parity  =   9, 
	stop    =   10
	};

#endif //__MAIN_H__
