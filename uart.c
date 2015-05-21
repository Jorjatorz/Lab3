/*-------------------------------------------------------------------
**
**  Fichero:
**    timer.h  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Contiene las definiciones de los prototipos de funciones
**    para la gesti�n de la uart 0 del chip S3C44BOX
**
**  Notas de dise�o:
**
**-----------------------------------------------------------------*/

#include "44b.h"
#include "utils.h"
#include "uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define MCLK 64000000

void Uart_Init(int baud)
{
	rPCONE = (rPCONE & ~(0xf<<2)) | (2<<4) | (2<<2); 	// PE[2] = RxD0, PE[1] = TxD0
	
    /* UART0 */
	// Modo normal, no paridad, 1b stop, 8b char

	rULCON0 &= (1<<9);
	rULCON0 |= 0x3;

	rULCON0 = 0x3;
	/*[1:0] Work length = 11 (8-bits)
	 * [2] Number of stop bit = 0 (One stop bit per frame)
	 * [5:3] Parity mode = 0xx (No parity
	 * [6] Infra- Red Mode = 0 (Normal mode operation)
	 * [7] Reserved
	 */


	// tx=nivel, rx=flanco, no rx-timeout ni rx-error, normal, int/polling
	rUCON0 = 0x205;

	rUBRDIV0=( (int)(MCLK/16./baud + 0.5) -1 ); // formula division de frecuencia

	// Desactivar FIFO
	rUFCON0 = 0x0;

	// Desactivar control de flujo
	rUMCON0 = 0x0;

}


// Funci�n que lee un caracter del puerto serie
// Realiza espera activa (bucle hasta que recibe un byte)
char Uart_Getch(void)
{
     // esperar a que el buffer contenga datos
	while((rUTRSTAT0 & 0x1) == 0x0) {}

	// devolver el caracter
	char* point = (char*)URXH0;

	return *point;

}


// Funci�n que env�a un byte por puerto serie
// Espera activa (espera hasta que se haya enviado)
void Uart_SendByte(char data)
{
	// Comprobamos si lo que se env�a es el caracter fin de l�nea
    if(data == '\n')		
	{
    	 // esperar a que THR se vacie
	   while (!(rUTRSTAT0 & 0x2));

	   // escribir retorno de carro (caracter \r)
	   char* point = UTXH0;
	   *point = 0xd;

	}
    // esperar a que THR se vacie
	while (!(rUTRSTAT0 & 0x2));

	// escribir data
	   char* point = UTXH0;
	   *point = data;
}


// Funci�n ya implementada: env�a una cadena de caracteres completa (hasta que encuentra \0)
void Uart_SendString(char *pt)
{
	//Mandar string con DMA (Esto es lo que tenemos que hacer en la parte 3)

	//BDCONn: Enable DMA request, No command
	rBDCON0 &= 0xF0; //4 last bits to 0: Enable DMA request and No command

	//BDISRCn: Configure Data Size(Byte), increment address and initial address = pt;
	rBDISRC0 = 0x10000000 ; //Configure Data size and increment address

    rBDISRC0 |= (unsigned int)pt; //Insert the address in the register;

    //BDIDESn: Transfer direction mode: M2IO, Increment address and initial address
    //rBDIDES0 = 0x50000000 ; //Configure M2IO and increment address
    rBDIDES0 = 0xD0000000 ; //Configure IO2IO and increment address
    rBDIDES0 |= (unsigned int)UTXH0; //Set destination register the UART;


    //unsigned int counter = sizeof(*pt)/ sizeof(char);

    const char *s;
    for (s = pt; *s; ++s);
    unsigned int counter = s - pt; //This works

    //BDICNT0 : N/A request, polling mode, disenable auto-reload, enable DMA, string length byte transfer
    rBDICNT0 = 0x4000000;
    rBDICNT0 |= counter;
    rBDICNT0 |= (1 << 20);
    //rBDICNT0 |= (1 << 31); //Activates DMA source selection to UART0 (N/A by default)
}

// Funci�n ya implementado: similar a printf pero enviando por puerto serie
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
}



