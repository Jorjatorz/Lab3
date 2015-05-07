/*-------------------------------------------------------------------
**
**  Fichero:
**    utils.c  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las implementaciones de funciones auxiliares
**
**  Notas de dise�o:
**    Equivale al fichero hom�nimo usado en Fundamentos de Computadores 
**
**-----------------------------------------------------------------*/

#include "utils.h"

#include "44b.h"
#define MCLK 64000000

//--------------------------------SYSTEM---------------------------------//
static int delayLoopCount=400;

void Delay(int time)
// time=0: adjust the Delay function by WatchDog timer.//
// time>0: the number of loop time//
// 100us resolution.//
{
	int i,adjust=0;
	if(time==0)
	{
		time=200;
		adjust=1;
		delayLoopCount=400;
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);	// 1M/64,Watch-dog,nRESET,interrupt disable//
		rWTDAT=0xffff;
		rWTCNT=0xffff;
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3)|(1<<5); // 1M/64,Watch-dog enable,nRESET,interrupt disable //
	}
	for(;time>0;time--)
		for(i=0;i<delayLoopCount;i++);
	if(adjust==1)
	{
		rWTCON=((MCLK/1000000-1)<<8)|(2<<3);
		i=0xffff-rWTCNT;   //  1count/16us?????????//
		delayLoopCount=8000000/(i*64);	//400*100/(i*64/200)   //
	}
}

void insertElementToQueue(tQueue* queue, int element)
{
	if(queue->elements < 4)
	{
		queue->_queue[queue->elements] = element;
		queue->elements = queue->elements + 1;
	}
	else
	{
		//Mover la arrey para la izquierda e insertar el nuevo elemento
		int i;
		for(i = 0; i < 3; i++)
		{
			queue->_queue[i] = queue->_queue[i + 1];
		}

		queue->_queue[3] = element;
	}
}
