#include "atask.h"

/*
 * thread handle entry for audio collection
 *
 *
 */
void thread_misc(cyg_addrword_t data) {
	dprintf("atask in...\n");
	while(1){
		//led_off(LEDG0);
		dprintf("atask...\n");
		cyg_thread_delay(500);
		//led_on(LEDG0);
		cyg_thread_delay(500);
	}

	dprintf("atask test end.\n");
}

