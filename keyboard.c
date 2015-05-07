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
#include "timer.h"

#define KEY_VALUE_MASK	0x0f


static int key_read( void );
static void keyboard_ISR(void) __attribute__ ((interrupt ("IRQ")));

static tQueue password;
static tQueue attempt;
static int gameCurrentMode = 0; //0 -> Asking Password, 1 -> Asking for match

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


	 //Inicializamos la cola y el display para mostrar una C
	 password.elements = 0;
	 attempt.elements = 0;
	 gameCurrentMode = 0;
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
	int key;

	/* Eliminar rebotes de presion */
	Delay(200);

	/* Escaneo de tecla */
	key = key_read();

	// En caso de error, key = -1
	/*if (key != -1)
		D8Led_digit(key); //Display the key on the D8 Led*/

	/*****************************************GAME LOGIC**********************************************************/
	//Turns off the leds
	led1_off();
	led2_off();

	//Password mode
	if(gameCurrentMode == 0)
	{
		//Si la tecla pulsada no es la F
		if(key != 15)
		{
			if (key > 0) //Is a valid key
				insertElementToQueue(&password, key);
			else {
				//There was an error reading the key -> Turn on the leds
				led1_on();
				led2_on();
			}

		}
		else
		{
			//Si la pass tiene 4 digitos msotrarlos cada segundo, sino no hacer nada
			if(password.elements == 4)
			{
				 timer_init(1); //Cada 1 segundo
				 timer_start(password);

				 while(!timer_isStop())
				 {}

				gameCurrentMode = 1; //Cambiar de modo
				D8Led_digit(15); //Mostar F
			}
		}
	}
	else
	{
		//Si la tecla pulsada no es la F
		if(key != 15)
		{
			insertElementToQueue(&attempt, key);
		}
		else
		{
			//Si el attempt tiene 4 digitos msotrarlos cada segundo, sino no hacer nada
			if(attempt.elements == 4)
			{
			 timer_init(1);
			 timer_start(attempt);

			 while(!timer_isStop())
			 {}
				 int equals = 1;
				 int i;
				 for(i = 0; i < 4; i++)
				 {
					 if(password._queue[i] != attempt._queue[i])
					 {
						 equals = 0;
						 break;
					 }
				 }

				 //Hacer una lista negativa para que el timer no la muestre
				 tQueue emptyQ;
				 emptyQ.elements = -1;
				 if(equals == 1)
				 {
					 D8Led_digit(10); //A
					 timer_init(2);
					 timer_start(emptyQ);
					 while(!timer_isStop())
					 {}
				 }
				 else
				 {
					 D8Led_digit(14); //E
					 timer_init(2);
					 timer_start(emptyQ);
					 while(!timer_isStop())
					{}
				 }


				 //Volvemos a empezar el juego
				 gameCurrentMode = 0; //Cambiar de modo
				 D8Led_digit(15); //Mostar C

			}
		}
	}
	/**************************************************************************************************************/

	/* Esperar a que la tecla se suelte */
	while (!(rPDATG & 0x02));

    /* Eliminar rebotes de depreson */
    Delay(200);

    /* Borrar interrupciones pendientes */
    rI_ISPC |= BIT_EINT1;
}

