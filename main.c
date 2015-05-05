/*-------------------------------------------------------------------
**
**  Fichero:
**    main.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Implementa el juego del mastermind
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#include "leds.h"
#include "D8Led.h"
#include "timer.h"
#include "keyboard.h"

int main(void)
{	
	intcont_init();
	D8Led_init();
	D8Led_digit( 0 );  	//Displays 0 in the 8Segment display
	keyboard_init(); 	//Initialize the configuration of the matrix keyboard
		
	while(1);
	return 0;
}

void intcont_init(void) {
	rINTCON = 0x1;			// - Modo vectorizado, FIQ deshabilitadas
	rINTMOD = 0x00000000;	// - Configurar todas las interrupciones por IRQ
	rI_ISPC = 0x3ffffff;	//  -Borramos pendientes por IRQ
	rF_ISPC = 0x3ffffff; 	//  -Borramos pendientes FIQ
	rEXTINTPND = 0x0;		// - Borrar EXTINPND
	rINTMSK = 0x4000000;	// - Enmascarar todas las líneas, pero activar BIT_GLOBAL
}
