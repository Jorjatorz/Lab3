/*-----------------------------------------------------------------
**
**  Fichero:
**    uart.h  10/6/2014
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
**---------------------------------------------------------------*/

#ifndef __UART_H__
#define __UART_H__

#define LF_char 0x0A	/* Caracter de salto de línea */
#define CR_char 0x0D    /* Caracter de retorno de carro */

void Uart_Init(int baud);
char Uart_Getch(void);
void Uart_SendByte(char data);
void Uart_SendString(char *pt);
void Uart_Printf(char *fmt,...);

#endif /* __UART_H__ */
