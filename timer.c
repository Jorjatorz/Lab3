/*-------------------------------------------------------------------
**
**  Fichero:
**    timer.h  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Contiene las definiciones de los prototipos de funciones
**    para la gestión del timer 0 del chip S3C44BOX
**
**  Notas de diseño:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "leds.h"
#include "D8led.h"
#include "timer.h"


static int timerStarted = 0;
static tQueue mQueue;
static int counter = 0;

static void timer_ISR(void) __attribute__ ((interrupt ("IRQ")));

static void timer_ISR( void )
{
	if(counter < 4)
	{
		D8Led_digit(mQueue._queue[counter]);
		counter++;
	}
	else
	{
		timer_stop();
	}

	rI_ISPC |= BIT_TIMER1;
}

void timer_init(int seconds)
{
	// Establecer la rutina de servicio para TIMER1
		pISR_TIMER1	 = timer_ISR;

	   /* f = MCLK / ((P+1) * D)
	    * para una interrupción cada 2 segundos: N * 1/f = 2 con N <= 65535
		* N * ((P+1)*D) / MCLK = 2 => P = 2 * MCLK / (N*D) - 1
		* Tomamos N como el mayor divisor de 2*CLK/D => para D = 8 y MCLK = 64MHz, N = 65200
	    * y P = 2*64000000/(62500*8) - 1 = 255, que cabe justo en los 8 bits de pre-escalado
	    */

		//MUX 1 seleccionado. Genera 1/4 (1 sec, 1/8 para 2 secs, 0x20)
		if(seconds == 1)
			rTCFG1 = 0x10;
		else
			rTCFG1 = 0x20;

		/* pre-escalado 255 */
		rTCFG0 = 0xFF00;

		/* Pre-Escalado 1 [15:8] : Estos ocho bits determinan el factor de pre-escalado
		 * de los temporizadores 2 y 3
		 *
		 *
		 * 255 = 1111 1111 -> rTCFG0
		 *
		 * 0000 1111 1111 0000 00000
		 * 	   [15]	   [8]
		 */

		/*Valor de cuenta 62500*/
		rTCNTB1 = 0xF424;  //62500 = 0xF424

		rTCMPB1 = 0;
		rI_ISPC |= BIT_TIMER1; //Limpiamos linea interrupcion del timer



		/* Paramos el timer al principio */
		timer_stop();
}


/*
 * Debe arrancar el timer1 para que genere una interrupción a los dos segundos
 * y en modo autoreload: valor de cuenta 62500, con pre-escalado y divisor
 * configurados en timer_init (no hace falta volver a configurarlo aquí).
 * Para arrancar el timer:
 * 	- activar bit de manual update
 * 	- descativar bit de manual update
 * 	- activar bits de auto-reload y start
 *
 * 	Los dos últimos pasos se pueden fusionar en una única instrucción C.
 *
 * 	Antes de arrancar deberíamos borrar posibles interrupciones pendientes y
 * 	desenmascarar.
 *
 * 	Los comentarios dan una idea de lo que hay que ir haciendo.
 */
void timer_start(tQueue queue)
{

	int BIT_MANUAL_T1 = 0x1<<9;
	int BIT_START_T1 =  0x1<<8;

    /* Activar bit de manual update */

	rTCON |= BIT_MANUAL_T1;

	/* Se borra el bit de interrupción pendiente del TIMER1 */

	rI_ISPC |= BIT_TIMER1;

	/* not manual update, start and auto-reload*/

	rTCON = BIT_START_T1 | 0x800;

	/* Se borra el bit de interrupción pendiente del TIMER2 */

	rI_ISPC |= BIT_TIMER1;

	timerStarted = 1;

	/* Desenmascara la linea del timer 2 y el bit global */
	rINTMSK &= ~BIT_TIMER1;
	rINTMSK &= ~BIT_GLOBAL;

	mQueue = queue;
	counter = 0;
}

/*
 * Debe parar el timer 2 y enmascarar su línea de interrupción */
void timer_stop(void)
{
	int BIT_MANUAL_T1 = 0x1<<9;

		/* Activar bit de manual update */

		rTCON |= BIT_MANUAL_T1;

		/* Se borra el bit de interrupción pendiente del TIMER1 */

		rI_ISPC |= BIT_TIMER1;

		//stop and auto-reload
		rTCON = 0x800;

		/* Se borra el bit de interrupción pendiente del TIMER1 */

		rI_ISPC |= BIT_TIMER1;

		timerStarted = 0;

		/* Enmascarar la linea del timer 1 */
		 rINTMSK |= BIT_TIMER1;
}

