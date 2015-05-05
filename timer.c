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



static void timer_ISR(void) __attribute__ ((interrupt ("IRQ")));

static void timer_ISR( void )
{
	
}

void timer_init(void)
{

}
