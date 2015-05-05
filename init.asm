/*-----------------------------------------------------------------
**
**  Fichero:
**    init.asm  10/6/2014
**
**    Estructura de Computadores
**    Dpto. de Arquitectura de Computadores y Autom�tica
**    Facultad de Inform�tica. Universidad Complutense de Madrid
**
**  Prop�sito:
**    Arranca un programa en C
**
**  Notas de dise�o:
**
**---------------------------------------------------------------*/

    .extern main
	.global start
	
	/*
	** Modos de operaci�n
	*/
	.equ MODEMASK, 0x1f		/* Para selecci�n de M[4:0] del CPSR */
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
	** Registro de m�scara de interrupci�n
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
	
	/* Desde modo SVC inicializa los SP de todos los modos de ejecuci�n privilegiados */
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
	/* Pasa a modo Undef e inicializa el SP_und */

    mrs	r0, cpsr				/* Llevamos el registro de estado a r0 */
    bic	r0, r0, #MODEMASK		/* Borramos los bits de modo de r0 */
    orr	r1, r0, #UNDMODE		/* A�adimos el c�digo del modo Undef y copiamos en r1 */
    msr	cpsr_c, r1				/* Escribimos el resultado en el registro de estado */
    ldr sp, =UNDSTACK			/* Copiamos la	direcci�n de comienzo de la pila */

	/* Pasa a modo Abort e inicializa el SP_abt */

    mrs	r0, cpsr				/* Llevamos el registro de estado a r0 */
    bic	r0, r0, #MODEMASK		/* Borramos los bits de modo de r0 */
    orr	r1, r0, #ABTMODE		/* A�adimos el c�digo del modo Abort y copiamos en r1 */
    msr	cpsr_c, r1				/* Escribimos el resultado en el registro de estado */
    ldr sp, =ABTSTACK			/* Copiamos la	direcci�n de comienzo de la pila */

    /* Pasa a modo IRQ e inicializa el SP_irq */

    mrs	r0, cpsr				/* Llevamos el registro de estado a r0 */
    bic	r0, r0, #MODEMASK		/* Borramos los bits de modo de r0 */
    orr	r1, r0, #IRQMODE		/* A�adimos el c�digo del modo Undef y copiamos en r1 */
    msr	cpsr_c, r1				/* Escribimos el resultado en el registro de estado */
    ldr sp, =IRQSTACK			/* Copiamos la	direcci�n de comienzo de la pila */

	/* Pasa a modo FIQ e inicializa el SP_fiq */

    mrs	r0, cpsr				/* Llevamos el registro de estado a r0 */
    bic	r0, r0, #MODEMASK		/* Borramos los bits de modo de r0 */
    orr	r1, r0, #FIQMODE		/* A�adimos el c�digo del modo Undef y copiamos en r1 */
    msr	cpsr_c, r1				/* Escribimos el resultado en el registro de estado */
    ldr sp, =FIQSTACK			/* Copiamos la	direcci�n de comienzo de la pila */

    /* Pasa a modo SVC e inicializa el SP_svc */

    mrs	r0, cpsr				/* Llevamos el registro de estado a r0 */
    bic	r0, r0, #MODEMASK		/* Borramos los bits de modo de r0 */
    orr	r1, r0, #SVCMODE		/* A�adimos el c�digo del modo Undef y copiamos en r1 */
    msr	cpsr_c, r1				/* Escribimos el resultado en el registro de estado */
    ldr sp, =SVCSTACK			/* Copiamos la	direcci�n de comienzo de la pila */

    mov pc, lr

	.end
