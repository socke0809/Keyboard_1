#ifndef __PS2_HW_H__
#define __PS2_HW_H__

#include <inttypes.h>


#define PS2_HW_FLAG_RCV_COMPLETE       	(1<<0)
#define PS2_HW_FLAG_TRANSF_COMPLETE 	(1<<1)
#define PS2_HW_FLAG_ERROR				(1<<3)
#define PS2_HW_FLAG_SENDING	    		(1<<6)
#define PS2_HW_FLAG_RECEIVING	    	(1<<7)



#define PS2_HW_DATA        PD3
#define PS2_HW_DATA_PIN    PIND
#define PS2_HW_DATA_DDR    DDRD
#define PS2_HW_DATA_PORT   PORTD

#define PS2_HW_CLK         PD2
#define PS2_HW_CLK_PIN     PIND
#define PS2_HW_CLK_DDR     DDRD
#define PS2_HW_CLK_PORT    PORTD

#define PS2_BUFFER_EMPTY (1<<0)
#define PS2_BUFFER_FULL	 (1<<1)

#define PS2_BUFFER_SIZE		16



struct ps2Buffer{
    uint8_t buffer[PS2_BUFFER_SIZE];

    uint8_t	read;
    uint8_t	write;
    uint8_t	ps2BufFlags;
};




void ps2_hw_init( void );

uint8_t ps2_hw_get_flags( void );

uint8_t ps2_get_buf_flags(struct ps2Buffer *buf);

int8_t ps2_hw_send_byte( uint8_t data);

int8_t ps2_hw_receive_byte(uint8_t *data);



#endif //__PS2_HW_H__
