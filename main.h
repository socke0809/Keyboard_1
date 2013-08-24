#ifndef __MAIN_H__
#define __MAIN_H__

#define PS2_FLAG_RCV_COMPLETE   	(1<<0)
#define PS2_FLAG_TRANSF_COMPLETE 	(1<<1)
#define PS2_FLAG_RECEIVING			(1<<2)
#define PS2_FLAG_ERROR				(1<<3)
#define PS2_FLAG_SENDING			(1<<4)


#define PS2_DATA        PD1
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_PORT   PORTD

#define PS2_CLK         PD2
#define PS2_CLK_PIN     PIND
#define PS2_CLK_DDR     DDRD
#define PS2_CLK_PORT    PORTD

enum ps2State {
	start   	=   0,
	data		=	1,
	parity  	=   9, 
	stop    	=   10,
	acknowledge	= 	11
	};

#endif //__MAIN_H__
