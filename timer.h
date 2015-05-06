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

#include "utils.h"

#ifndef _TIMER_H_
#define _TIMER_H_

void timer_init( int seconds );

void timer_start(tQueue queue);

void timer_stop(void);

#endif
