/**
 *
 * @file main.c
 *
 * @date May 24, 2017
 * @author hamster
 *
 * @brief This is the entry point for the DC801 DC26 party badge
 *
 */

#include "main.h"
#include "games/mage/mage.h"
#include "FrameBuffer.h"
#include "EnginePanic.h"
#include "fonts/Monaco9.h"

#ifdef DC801_EMBEDDED
//only init QSPI if we're in embedded mode:
#include "qspi.h"
QSPI qspiControl;

#endif

#include "test.h"

#ifdef DC801_DESKTOP
volatile sig_atomic_t application_quit = 0;

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		debug_print("received SIGINT\n");
		application_quit = 1;
	}
}

#endif

#define XVAL(x) #x
#define VAL(x)  XVAL(x)

/*
#pragma message ("TIMER_ENABLED=" VAL(TIMER_ENABLED))
#pragma message ("TIMER1_ENABLED=" VAL(TIMER1_ENABLED))
#pragma message ("NRFX_TIMER_ENABLED=" VAL(NRFX_TIMER_ENABLED))
#pragma message ("NRFX_TIMER1_ENABLED=" VAL(NRFX_TIMER1_ENABLED))
*/

/**
 * Initialize the speaker
 */
static void speaker_init(void){
	// Setup the nau8810 later -Tim
	//nrf_gpio_cfg_output(SPEAKER);
}

/**
 * Initialize the logging backend for logging over JTAG
 */
static void log_init(void){
	ret_code_t err_code = NRF_LOG_INIT(NULL);
	APP_ERROR_CHECK(err_code);
	NRF_LOG_DEFAULT_BACKENDS_INIT();
	NRF_LOG_INFO("--------------SYSTEM REBOOTED--------------");
	NRF_LOG_ERROR("Error Logging to hardware UART enabled.");
	NRF_LOG_INFO("Debug Logging to hardware UART enabled.");
}

/**
 * Initialize the QSPI ROM object
 */
static void rom_init(void){
	#ifdef DC801_EMBEDDED
	if(!qspiControl.init()){
		ENGINE_PANIC("Failed to init qspiControl.");
	}
	#endif
}

//this lets me test if the framebuffer is working:
void test_screen(){
	FrameBuffer *test_canvas;
	test_canvas = p_canvas();
	test_canvas->clearScreen(COLOR_PURPLE);
	//util_gfx_draw_raw_file("hcrn/display.raw", 0, 0, 128, 128, NULL, true, NULL);
	test_canvas->blt();
	nrf_delay_ms(1000);
	test_canvas->clearScreen(COLOR_DARKBLUE);
	test_canvas->blt();
	nrf_delay_ms(16);
	test_canvas->clearScreen(0x18e5);
	test_canvas->blt();
	nrf_delay_ms(16);
	test_canvas->clearScreen(COLOR_DARKBLUE);
	test_canvas->blt();
	nrf_delay_ms(16);
}

//this will blink the LED next to a button, or turn off all LEDs when a joystick button is held down.
void test_keyboard(){
	ledsOff();
	bool led_brightness[LED_COUNT];
	for(int i=0; i<LED_COUNT; i++) {led_brightness[i] = false;}
	//need to keep updating in a loop for it to work:
	while(1){
		//get an updated key mask every loop:
		EngineHandleInput();
		if(EngineInput_Activated.mem0)   { ledSet(LED_MEM0,   (led_brightness[LED_MEM0]  = !led_brightness[LED_MEM0]   )? 0xff : 0x00);}
		if(EngineInput_Activated.mem1)   { ledSet(LED_MEM1,   (led_brightness[LED_MEM1]  = !led_brightness[LED_MEM1]   )? 0xff : 0x00);}
		if(EngineInput_Activated.mem2)   { ledSet(LED_MEM2,   (led_brightness[LED_MEM2]  = !led_brightness[LED_MEM2]   )? 0xff : 0x00);}
		if(EngineInput_Activated.mem3)   { ledSet(LED_MEM3,   (led_brightness[LED_MEM3]  = !led_brightness[LED_MEM3]   )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_128){ ledSet(LED_BIT128, (led_brightness[LED_BIT128]= !led_brightness[LED_BIT128] )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_64) { ledSet(LED_BIT64,  (led_brightness[LED_BIT64] = !led_brightness[LED_BIT64]  )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_32) { ledSet(LED_BIT32,  (led_brightness[LED_BIT32] = !led_brightness[LED_BIT32]  )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_16) { ledSet(LED_BIT16,  (led_brightness[LED_BIT16] = !led_brightness[LED_BIT16]  )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_8)  { ledSet(LED_BIT8,   (led_brightness[LED_BIT8]  = !led_brightness[LED_BIT8]   )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_4)  { ledSet(LED_BIT4,   (led_brightness[LED_BIT4]  = !led_brightness[LED_BIT4]   )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_2)  { ledSet(LED_BIT2,   (led_brightness[LED_BIT2]  = !led_brightness[LED_BIT2]   )? 0xff : 0x00);}
		if(EngineInput_Activated.bit_1)  { ledSet(LED_BIT1,   (led_brightness[LED_BIT1]  = !led_brightness[LED_BIT1]   )? 0xff : 0x00);}
		if(EngineInput_Activated.op_xor) { ledSet(LED_XOR,    (led_brightness[LED_XOR]   = !led_brightness[LED_XOR]    )? 0xff : 0x00);}
		if(EngineInput_Activated.op_add) { ledSet(LED_ADD,    (led_brightness[LED_ADD]   = !led_brightness[LED_ADD]    )? 0xff : 0x00);}
		if(EngineInput_Activated.op_sub) { ledSet(LED_SUB,    (led_brightness[LED_SUB]   = !led_brightness[LED_SUB]    )? 0xff : 0x00);}
		if(EngineInput_Activated.op_page){ ledSet(LED_PAGE,   (led_brightness[LED_PAGE]  = !led_brightness[LED_PAGE]   )? 0xff : 0x00);}
		if(EngineInput_Activated.hax)    { ledSet(LED_HAX,    (led_brightness[LED_HAX]   = !led_brightness[LED_HAX]    )? 0xff : 0x00);}
		nrf_delay_ms(16);
		//escape by holding xor and pressing mem0:
		if(EngineInput_Buttons.op_xor && EngineInput_Activated.mem0){
			break;
		}
	}
}

//this tests reading and writing to the ROM chip using QSPI.
void test_rom(){
	#ifdef DC801_EMBEDDED
	uint8_t erased_array[9] {255, 255, 255, 255, 255, 255, 255, 255, 0};
	char test_array[9] = "GOATBOAT";
	char test_rx_array[36] {'X','X','X','X','X','X','X','X',0};
	char address_message[128];
	EngineROM_Read(
		0,
		ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH,
		(uint8_t *)test_rx_array,
		"Failed to read timestamp from ROM"
	);
	p_canvas()->clearScreen(COLOR_BLACK);
	sprintf(
		address_message,
		"First few bytes after startup:\n%03d %03d %03d %03d %03d %03d %03d %03d \n",
		test_rx_array[0],
		test_rx_array[1],
		test_rx_array[2],
		test_rx_array[3],
		test_rx_array[4],
		test_rx_array[5],
		test_rx_array[6],
		test_rx_array[7]
	);
	p_canvas()->printMessage(
		address_message,
		Monaco9,
		COLOR_WHITE,
		32,
		8
	);
	// p_canvas()->printMessage(
	// 	"Starting to write zeros",
	// 	Monaco9,
	// 	COLOR_WHITE,
	// 	32,
	// 	48
	// );
	p_canvas()->blt();
	/*
	while(qspiControl.isBusy()){
		// is very busy
	}
	// disabling erase and write unless specifically needed for testing.
	uint32_t zero = 0;
	for (uint32_t i = 0; i < 262145; i += 4) {
		EngineROM_Write(
			i,
			4,
			(uint8_t *)&zero,
			"Could not write zeros to ROM"
		);
	}
	EngineROM_Read(
		0,
		ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH,
		(uint8_t *)test_rx_array,
		"Failed to read timestamp from ROM"
	);
	sprintf(
		address_message,
		"First few bytes read after write:\n%03d %03d %03d %03d %03d %03d %03d %03d \n",
		test_rx_array[0],
		test_rx_array[1],
		test_rx_array[2],
		test_rx_array[3],
		test_rx_array[4],
		test_rx_array[5],
		test_rx_array[6],
		test_rx_array[7]
	);
	p_canvas()->printMessage(
		address_message,
		Monaco9,
		COLOR_WHITE,
		32,
		64
	);
	p_canvas()->blt();
	p_canvas()->printMessage(
		"Erasing one BLOCK_SIZE_256K",
		Monaco9,
		COLOR_WHITE,
		32,
		96
	);
	p_canvas()->blt();
	if(!qspiControl.erase(tBlockSize::BLOCK_SIZE_256K, 0)){
		ENGINE_PANIC("Failed to send erase comand.");
	}
	while(qspiControl.isBusy()){
		// is very busy
	}
	EngineROM_Read(
		0,
		ENGINE_ROM_MAGIC_STRING_LENGTH + ENGINE_ROM_TIMESTAMP_LENGTH,
		(uint8_t *)test_rx_array,
		"Failed to read timestamp from ROM"
	);
	sprintf(
		address_message,
		"First few bytes read after sector erase:\n%03d %03d %03d %03d %03d %03d %03d %03d \n",
		test_rx_array[0],
		test_rx_array[1],
		test_rx_array[2],
		test_rx_array[3],
		test_rx_array[4],
		test_rx_array[5],
		test_rx_array[6],
		test_rx_array[7]
	);
	p_canvas()->printMessage(
		address_message,
		Monaco9,
		COLOR_WHITE,
		32,
		112
	);
	p_canvas()->printMessage(
		"Starting to read how many ones in a row",
		Monaco9,
		COLOR_WHITE,
		32,
		136
	);
	p_canvas()->blt();
	for (uint32_t i = 0; i < ENGINE_ROM_QSPI_CHIP_SIZE; i += 4) {
		EngineROM_Read(
			i,
			4,
			(uint8_t *)&zero,
			"Could not read from ROM"
		);
		if(zero == 0) {
			sprintf(
				address_message,
				"Address of first zero is: %d\n",
				i
			);
			p_canvas()->printMessage(
				address_message,
				Monaco9,
				COLOR_WHITE,
				32,
				144
			);
			p_canvas()->blt();
			break;
		}
	}
	*/

	// p_canvas()->printMessage(
	// 	"Starting chip erase",
	// 	Monaco9,
	// 	COLOR_WHITE,
	// 	32,
	// 	32
	// );
	// p_canvas()->blt();
	// //start by erasing the whole chip:
	// while(qspiControl.isBusy()){
	// 	// is very busy
	// }
	// if(!qspiControl.chipErase()){
	// 	ENGINE_PANIC("Failed to erase ROM Chip.");
	// }

	// debug_print("Verifying Erase...");
	// if(EngineROM_Verify(0, 8, erased_array) != ENGINE_ROM_VERIFY_SUCCESS){
	// 	ENGINE_PANIC("Verification of erase failed.");
	// }
	// debug_print("Writing %s to ROM chip...", test_array);
	// EngineROM_Write(
	// 	0,
	// 	8,
	// 	(uint8_t *)test_array,
	// 	"Failed to write to ROM."
	// );
	// EngineROM_Read(
	// 	0,
	// 	8,
	// 	(uint8_t *)test_rx_array,
	// 	"QSPI read failed."
	// );

	// p_canvas()->printMessage(
	// 	"We got to the end",
	// 	Monaco9,
	// 	COLOR_WHITE,
	// 	32,
	// 	160
	// );

	p_canvas()->blt();
	#endif
}

/**
 * @brief Main app
 * @return Not used
 */
int main(void){

	#ifdef DC801_DESKTOP

	signal(SIGINT, sig_handler);

	#endif

	// Setup the system
	log_init();

	// Timers
	app_timer_init();

#ifdef DC801_EMBEDDED

	//USB serial
	//usb_serial_init();

	//keyboard controls all hardware buttons on this badge
	keyboard_init();

	//this function will set up the NAU8810 chip to play sounds
	//speaker_init();

	// BLE
	//gap_params_init();
	ble_stack_init();
	scan_start();

	// Init the display
	ili9341_init();
	ili9341_start();
	util_gfx_init();

	//Init the SD Card
	if(!util_sd_init()){
		util_sd_error();
	}

	//QSPI ROM Chip
	rom_init();

	// Init the random number generator
	nrf_drv_rng_init(NULL);

	// Setup the battery monitor
	//adc_configure();
	//adc_start();

	// Setup the UART
	//uart_init();

	// Setup I2C
	twi_master_init();

	//EEpwm_init();

	const char* ble_name = "TheMage801"; // must be 10char
	debug_print("advertising user: %s", ble_name);
	advertising_setUser(ble_name);
	ble_adv_start();
#endif

	// Setup LEDs
	ledInit();
	ledsOn();

	//morse isn't used on this badge yet...
	//morseInit();

	// Configure the systick
	sysTickStart();

	// Boot! Boot! Boot!
	debug_print("Booted!\n");
	// printf goes to the RTT_Terminal.log after you've fired up debug.sh

#ifdef DC801_EMBEDDED
	//this just prints the screen black for a bit before continuing.
	//Feel free to delete the function once everything is working -Tim
	test_screen();

	//this tests button inputs by blinking LEDs.
	//it's blocking, so comment it out when not actively testing.
	//Feel free to delete the function once everything is working -Tim
	//test_keyboard();

	//this tests reading and writing to the ROM chip using QSPI.
	//Feel free to delete the function once everything is working -Tim
	test_rom();
#endif

#if defined(TEST) || defined(TEST_ALL)
	DC801_Test::Test();
	break;
#else
	MAGE();
#endif

#ifdef DC801_DESKTOP
	debug_print("Exiting gracefully...\n");
	return 0;
#endif

#ifdef DC801_EMBEDDED
	while (1)
	{
		// If we make it here - we shouldn't - but if, reset the badge
		NVIC_SystemReset();
	}
#endif
}
