/*-------------------------------------------------------------------
**
**  Fichero:
**    keyboard.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las implementaciones de las funciones
**    para la gestión del teclado de la placa de prototipado
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "keyboard.h"
#include "timer.h"

#define KEY_VALUE_MASK	0x0f


static int key_read( void );
static void keyboard_ISR(void) __attribute__ ((interrupt ("IRQ")));

int key = -1;

void keyboard_init( void )
{

	/* Configurar puerto G para interrupciones del teclado */

	rPCONG = 0xffff; //All the pins interrupts in their lines

	/* Establecer ISR de teclado */
	pISR_EINT1 = keyboard_ISR;

	/*Borrar interrupciones antes habilitar*/

	 rI_ISPC = 0x3ffffff;	//  -Borramos pendientes por IRQ
	 rF_ISPC = 0x3ffffff; 	//  -Borramos pendientes FIQ


    /* Desenmascara la línea del teclado y el bit global */

	 rINTMSK &= ~BIT_GLOBAL;	//Unmask global bit
	 rINTMSK &= ~BIT_EINT1;	 	//Unmask keybouard line


	 //Inicializamos el display para mostrar una C
	 D8Led_digit(12); //Display C

}

static int key_read( void )
{
	int value;
	char temp;

	/* leer linea 1 */
	temp = *(KEYBADDR+0xfd);
	temp = temp & KEY_VALUE_MASK;
	if (temp  != KEY_VALUE_MASK)
	{
		if( temp == 0x0E )
			value = 3;
		else if( temp == 0x0D )
			value = 2;
		else if( temp == 0x0B )
			value = 1;
		else if( temp == 0x07 )
			value = 0;
		return value;
	}
	
	/* linea 2 */
	temp = *(KEYBADDR+0xfb);
	temp = temp & KEY_VALUE_MASK;
	if (temp  != KEY_VALUE_MASK)
	{
		if( temp == 0x0E )
			value = 7;
		else if( temp == 0x0D )
			value = 6;
		else if( temp == 0x0B )
			value = 5;
		else if( temp == 0x07 )
			value = 4;
		return value;
	}
	
	/* linea 3 */
	temp = *(KEYBADDR+0xf7);
	temp = temp & KEY_VALUE_MASK;
	if (temp  != KEY_VALUE_MASK)
	{
		if( temp == 0x0E )
			value = 0xb;
		else if( temp == 0x0D )
			value = 0xa;
		else if( temp == 0x0B )
			value = 9;
		else if( temp == 0x07 )
			value = 8;
		return value;
	}
	
	/* linea 4 */
	temp = *(KEYBADDR+0xef);
	temp = temp & KEY_VALUE_MASK;
	if (temp  != KEY_VALUE_MASK)
	{
		if( temp == 0x0E )
			value = 0xf;
		else if( temp == 0x0D )
			value = 0xe;
		else if( temp == 0x0B )
			value = 0xd;
		else if( temp == 0x07 )
			value = 0xc;
		return value;
	}
	return -1;
}

static void keyboard_ISR(void)
{
	/* Eliminar rebotes de presion */
	Delay(200);

	/* Escaneo de tecla */
	key = key_read();

	// En caso de error, key = -1
	/*if (key != -1)
		D8Led_igit(key); //Display the key on the D8 Led*/
	if(key == -1)
	{
		//There was an error reading the key -> Turn on the leds
		led1_on();
		led2_on();
	}
	else
	{
		/*****************************************GAME LOGIC**********************************************************/
		//Turns off the leds
		led1_off();
		led2_off();
	}

	/**************************************************************************************************************/

	/* Esperar a que la tecla se suelte */
	while (!(rPDATG & 0x02));

    /* Eliminar rebotes de depreson */
    Delay(200);

    /* Borrar interrupciones pendientes */
    rI_ISPC |= BIT_EINT1;
}

