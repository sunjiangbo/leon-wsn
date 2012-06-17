#include "includes.h"
#include "led.h"

#include "systask.h"
#include "vtask.h"
#include "atask.h"
/*task stack and handle*/
/*systask*/
#define SYSTSK_STK_SIZE	51200
#define VTASK_STK_SIZE	51200 //204800
static cyg_handle_t systsk_hdl;
static cyg_thread systsk_thr;
static char systsk_stack[SYSTSK_STK_SIZE];

/*video task*/
static cyg_handle_t vtsk_hdl;
static cyg_thread vtsk_thr;
static char vtsk_stack[VTASK_STK_SIZE];

/*global parameters*/
cyg_mutex_t cliblock;
//int8u_t txbuf[128] = { 0 };
//int8u_t rxbuf[144] = { 0 };


/*extern parameters*/
extern const short music_data[FILELEN];


/*
 * Author: xiaoyang
 * Created: 2011-9-22
 * Parameters:
 * Description:SW & HW init
 */
void system_init(void) {
	dprintf("system init begin. v3.2\n");
	/*gpio & aux init*/
	/*
	 * pio[0-11] vin_data
	 * pio[12-16] vga
	 * pio[17-22] vin_if
	 */
	//0-23 aux0
	gpio_init(PORTA, 0x00FF); //set io[0-7] as vin pins(aux0[0-7])
	gpio_init(PORTC,0x007E); //set gpio[17-22] vin_if,oters gpio
	gpio_init(PORTB, 0x000F); //set io[8-11] vin,[12-15] as gpio pins
	//gpio_init_gpio(PORTB, 0x000F); 
	/*
	//------------------------------------------------------------------
	// GPIO add by xiaoyang
	//------------------------------------------------------------------
	assign	pio_in[12]	=   GPIO_0[ 12] ;  assign	GPIO_0[ 12] = pio_oen[12]	? 1'bz: pio_out[12]	; 
	assign	pio_in[13]	=   GPIO_0[ 13] ;  assign	GPIO_0[ 13] = pio_oen[13]	? 1'bz: pio_out[13]	; 	
	assign	pio_in[14]	=   GPIO_0[ 14] ;  assign	GPIO_0[ 14] = pio_oen[14]	? 1'bz: pio_out[14]	; 	
	assign	pio_in[15]	=   GPIO_0[ 15] ;  assign	GPIO_0[ 15] = pio_oen[15]	? 1'bz: pio_out[15]	; 	
	assign	pio_in[16]	=   GPIO_0[ 16] ;  assign	GPIO_0[ 16] = pio_oen[16]	? 1'bz: pio_out[16]	; 	
	//GOIO 39-43
	assign	pio_in[39]  =   GPIO_0[17] ;  assign	GPIO_0[17] = pio_oen[39]? 1'bz: pio_out[39] ;	 
	assign	pio_in[40]  =   GPIO_0[18] ;  assign	GPIO_0[18] = pio_oen[40]? 1'bz: pio_out[40] ;	 
	assign	pio_in[41]  =   GPIO_0[19] ;  assign	GPIO_0[19] = pio_oen[41]? 1'bz: pio_out[41] ;	 
	assign	pio_in[42]  =   GPIO_0[20] ;  assign	GPIO_0[20] = pio_oen[40]? 1'bz: pio_out[42] ;	 
	assign	pio_in[43]  =   GPIO_0[21] ;  assign	GPIO_0[21] = pio_oen[41]? 1'bz: pio_out[43] ;	 
	*/
	gpio_init_gpio(PORTF, 0x0C);
	/*gpio 40 test*/
	gpio_make_in(PORTF,0x01);
	gpio_make_out(PORTF,0x02);
	
	/*led*/
	//led_init();
	//led_off(LEDG0);
	//led_off(LEDG1);

	/*spi init for RF*/
	printf("spi init begin ..\n");
	//spi2_init();
	//spi2_test();
	printf("spi init end...\n");

	/*RF init*/
	//RF_CHIP_INITIALIZE();
	//printf("RF_CHIP_INITIALIZE over..\n");
	//netconfig();
	printf("netconfig over..\n");
	
	/*RF test*/
	//#if USE_SD
	printf("FS test begin...\n");
	//fs_test();
	printf("FS test end...\n");
	//#endif
	
	/*ck_gen vin open*/
	ck_enable(CTR_VIN);	
	//gpio_make_out(PORTB, SCCB_CLK);
	//gpio_make_out(PORTB, SCCB_DATA);
	//gpio_set(PORTB,SCCB_CLK);
	//gpio_clr(PORTB,SCCB_DATA);
	
	/*big or little endien check*/
	//testing();
	if(check_cpu_endien() == BIG_ENDIEND){
		printf("CPU endiend:big.\n");
	}else{
		printf("CPU endiend:little.\n");
	}
	printf("system init end.\n");
}


/*
 * Author: xiaoyang
 * Created: 2011-9-22
 * Parameters:
 * Description:entry of program
 */
int main(void) {
	unsigned int i = 0;
	/*configueration*/
	system_init();

	/*ecos work*/
	dprintf("\neCos start:[ok]\n");

	/*init and setup working threads*/
	cyg_thread_create(
		3, 
		systsk_handle,
		(cyg_addrword_t) 0, 
		"sysctrl_thread",
		(void *) systsk_stack, 
		SYSTSK_STK_SIZE, 
		&systsk_hdl, 
		&systsk_thr);
	
		cyg_thread_create(
		4,/*priority*/ 
		vtsk_handle,/*entry*/ 
		(cyg_addrword_t) 0,/*entry data*/
		"video_thread",/*thread name*/
		(void *) vtsk_stack,/*stack base*/ 
		VTASK_STK_SIZE,/*stack size*/
		&vtsk_hdl, /*thread handle*/
		&vtsk_thr);/*thread entity address*/

	/*setup*/
	cyg_thread_resume(vtsk_hdl);
	cyg_thread_resume(systsk_hdl);
	cyg_scheduler_start();

	dprintf("@ok,main exit(0).\n");

	/*do some clean works*/

	return 0;
}

