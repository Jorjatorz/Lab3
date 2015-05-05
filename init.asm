/*-----------------------------------------------------------------
**
**  Fichero:
**    init.asm  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Automática
**    Facultad de Informática. Universidad Complutense de Madrid
**
**  Propósito:
**    Arranca un programa en C
**
**  Notas de diseño:
**
**---------------------------------------------------------------*/

    .extern main
	.global start
	
	/*
	** Modos de operación
	*/
	.equ MODEMASK, 0x1f		/* Para selección de M[4:0] del CPSR */
	.equ USRMODE,  0x10
	.equ FIQMODE,  0x11
	.equ IRQMODE,  0x12
	.equ SVCMODE,  0x13
	.equ ABTMODE,  0x17
	.equ UNDMODE,  0x1b
	.equ SYSMODE,  0x1f

	/*
	** Direcciones de las bases de las pilas del sistema 
	*/
	.equ USRSTACK, 0xc7ff000   	
	.equ SVCSTACK, 0xc7ff100
	.equ UNDSTACK, 0xc7ff200
	.equ ABTSTACK, 0xc7ff300
	.equ IRQSTACK, 0xc7ff400
	.equ FIQSTACK, 0xc7ff500

	/*
	** Registro de máscara de interrupción
	*/
	.equ rINTMSK,    0x1e0000c

start:

	/* Pasa a modo supervisor */
    mrs	r0, cpsr
    bic	r0, r0, #MODEMASK
    orr	r1, r0, #SVCMODE
    msr	cpsr_c, r1 

	/* Enmascara interrupciones */
	ldr r0, =rINTMSK
	ldr r1, =0x1fffffff
    str r1, [r0]

	/* Habilita linea IRQ del CPSR */
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0
	
	/* Desde modo SVC inicializa los SP de todos los modos de ejecución privilegiados */
    bl InitStacks
	
	/* Desde modo SVC cambia a modo USR e inicializa el SP_usr */
	mrs r0, cpsr
	bic r0, r0, #MODEMASK
	orr r1, r0, #USRMODE  
	msr cpsr_c, r1
	ldr sp, =USRSTACK

    mov fp, #0

    bl main

End:
    B End

InitStacks:
	@ Completar
    mov pc, lr

	.end

