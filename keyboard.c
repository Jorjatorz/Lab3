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
#include "utils.h"
#include "keyboard.h"

#define KEY_VALUE_MASK	0x0f


static int key_read( void );
static void keyboard_ISR(void) __attribute__ ((interrupt ("IRQ")));

void keyboard_init( void )
{

	/* Configurar puerto G para interrupciones del teclado */

	/* Establecer ISR de teclado */


	/*Borrar interrupciones antes habilitar*/


    /* Desenmascara la línea del teclado y el bit global */

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
	int key;

	/* Eliminar rebotes de presion */
	Delay(2000);

	/* Escaneo de tecla */
	key = key_read();

	// En caso de error, key = -1
	// COMPLETAR !!!!

	/* Esperar a que la tecla se suelte */
	while (!(rPDATG & 0x02));

    /* Eliminar rebotes de depreson */
    Delay(2000);

    /* Borrar interrupciones pendientes */

}

