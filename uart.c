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
**    para la gestión de la uart 0 del chip S3C44BOX
**
**  Notas de diseño:
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


// Función que lee un caracter del puerto serie
// Realiza espera activa (bucle hasta que recibe un byte)
char Uart_Getch(void)
{
     // esperar a que el buffer contenga datos
	while((rUTRSTAT0 & 0x1) == 0x0) {}

	// devolver el caracter
	char* point = (char*)URXH0;

	return *point;

}


// Función que envía un byte por puerto serie
// Espera activa (espera hasta que se haya enviado)
void Uart_SendByte(char data)
{
	// Comprobamos si lo que se envía es el caracter fin de línea
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


// Función ya implementada: envía una cadena de caracteres completa (hasta que encuentra \0)
void Uart_SendString(char *pt)
{
	//Mandar string con DMA (Esto es lo que tenemos que hacer en la parte 3)

	//BDCONn: Enable DMA request, No command
	rBDCON0 &= 0x0; //4 last bits to 0: Enable DMA request and No command

	//BDISRCn: Configure Data Size(Byte), increment address and initial address = pt;
	rBDISRC0 = 0x1000000 ; //Configure Data size and increment address
    pt &= 0xFFFFFFF; //Adapts the initial address
    rBDISRC0 |= pt; //Insert the address in the register;

    //BDIDESn: Transfer direction mode: IO2M, Increment address and initial address
    rBDIDES0 = 0x90000000 ; //Configure IO2M and increment address
    pt &= 0xFFFFFFF; //Adapts the initial address
    rBDIDES0 |= pt; //Insert the address in the register;

    //BDICNT0 and BDCCNT0: UART0 request, polling mode, enable auto-reload, enable DMA, 1 byte transfer
    rBDICNT0 = 0x84300001; //Reserved fields are left in their initial state
    rBDCCNT0 = 0x84300001; //Reserved fields are left in their initial state


	/*
	 // Mandar byte a byte hasta completar string sin DMA
    while (*pt)
    	Uart_SendByte(*pt++);

    	*/
}

// Función ya implementado: similar a printf pero enviando por puerto serie
void Uart_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString(string);
    va_end(ap);
}



