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
#include "uart.h"

void intcont_init(void) {
	rINTCON = 0x1;			// - Modo vectorizado, FIQ deshabilitadas
	rINTMOD = 0x0;	// - Configurar todas las interrupciones por IRQ
	rI_ISPC = 0x3ffffff;	//  -Borramos pendientes por IRQ
	rF_ISPC = 0x3ffffff; 	//  -Borramos pendientes FIQ
	rEXTINTPND = 0x0;		// - Borrar EXTINPND
	rINTMSK = 0x7FFFFFF;	// - Enmascarar todas las líneas, pero activar BIT_GLOBAL
}

int main(void)
{	
	intcont_init();
	D8Led_init();
	D8Led_digit( 0 );  	//Displays 0 in the 8Segment display
	keyboard_init(); 	//Initialize the configuration of the matrix keyboard
	leds_init(); //Initializes the leds
	Uart_Init(115200); //Initliaze the UART to read 115200 bits per second
		
	tQueue password;
	tQueue attempt;
	 password.elements = 0;
	 attempt.elements = 0;
	 gameCurrentMode = 0;

	while(1)
	{
		//Password mode
			if(gameCurrentMode == 0)
			{
				//Si la tecla pulsada no es la F
				while(key != 15)
				{
					if (key >= 0 && key != 15) //Is a valid key
					{
						insertElementToQueue(&password, key);
						key = -1;
					}
				}


					//Si la pass tiene 4 digitos msotrarlos cada segundo, sino no hacer nada
					if(password.elements == 4)
					{
						 timer_init(1); //Cada 1 segundo
						 timer_start(password);

						 while(!timer_isStop())
						 {

						 }

						 key = -1;

						//gameCurrentMode = 1; //Cambiar de modo
						//D8Led_digit(15); //Mostar F
					}
					else
					{
						key = -1;
					}
			}

			else
			{
				//Se ha cambiado de modo, enviamos el mensaje
				Uart_Printf("Enter a key");

				//Leemos hasta que encontramos \n
				int finLinea = 0;
				while(!finLinea)
				{
					//Leemos del p. serie
					char readC = Uart_Getch();

					if (readC != '\n')
					{
						insertElementToQueue(&attempt, readC - 48); //Char to int;
					}
					else
					{
						finLinea = 1;
					}
				}


					//Si el attempt tiene 4 digitos msotrarlos cada segundo, sino no hacer nada
					if(attempt.elements == 4)
					{
					 timer_init(1);
					 timer_start(attempt);

					 	while(!timer_isStop()){}

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
						 if(equals == 1) //Attempt correcto!
						 {
							 //Se ha acertado la contraseña
							 Uart_Printf("CORRECT");
							 D8Led_digit(10); //A
							 timer_init(2);
							 timer_start(emptyQ);
							 while(!timer_isStop()){}
						 }
						 else
						 {
							 Uart_Printf("ERROR");
							 D8Led_digit(14); //E
							 timer_init(2);
							 timer_start(emptyQ);
							 while(!timer_isStop()){}
						 }

						 gameCurrentMode = 1;
						 key = -1;
						 attempt.elements = 0;

						 D8Led_digit(15);

						 //Volvemos a empezar el juego
						// gameCurrentMode = 0; //Cambiar de modo
						 //D8Led_digit(15); //Mostar C

					}
					else
					{
						key = -1;
					}

			}
	}


	return 0;
}
