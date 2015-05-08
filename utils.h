/*-------------------------------------------------------------------
**
**  Fichero:
**    utils.h  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de funciones
**    auxiliares
**
**  Notas de dise�o:
**    Equivale al fichero hom�nimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#ifndef UTILS_H_
#define UTILS_H_

void Delay( int time );

typedef struct
{
	short int _queue[4];

	short int elements;
} tQueue;

void insertElementToQueue(tQueue* queue, int element);

static int gameCurrentMode = 0; //0 -> Asking Password, 1 -> Asking for match

#endif
