#include <avr/interrupt.h>

enum ps2ReceiveState = {
	start   =   0, 
	parity  =   9, 
	stop    =   10
};
char clk;
char data;


ISR( INT0_vect )
{
	if(clk == 0){
		if((ps2ReceiveState > 0) && (ps2ReceiveState < 9)){ 
			//TODO: read data
			ps2ReceiveState++;
			}
		else if(Ps2ReceiveState == 9){			//parity bit
			//TODO: parity bit 
			ps2ReceiveState++;
			}
		else{									//stop bit
			ps2ReceiveState = 0;
			}
		}
	
	


int main()
{
    while(1);
}
