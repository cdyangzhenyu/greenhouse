#ifndef _RELAY_H
#define _RELAY_H


#define PF5 		PFout(5)   	//PF5
#define PF6 		PFout(6)		//PF6 
#define PF7 		PFout(7)		//PF7
#define PF8 	  PFout(8)		//PF8

#define PF0 		PFout(0)   	//PF5
#define PF2 		PFout(2)		//PF6 
#define PF3 		PFout(3)		//PF7
#define PF4 	  PFout(4)		//PF8


void relay_init(void);
void relay_on(void);
void relay_off(void);

#endif


