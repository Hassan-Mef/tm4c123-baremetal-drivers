/******************************************************************************
 * FILENAME    : startup.s
 * DESCRIPTION : Startup code for TM4C123GH6PM
 *
 * AUTHOR      : Hassan
 ******************************************************************************/

.syntax unified
.cpu cortex-m4
.thumb

.section .isr_vector,"a",%progbits

.global VectorTable

.align 2

.global Reset_Handler
.global Default_Handler
.global NMI_Handler
.global HardFault_Handler
.global MemManage_Handler
.global BusFault_Handler
.global UsageFault_Handler
.global SVC_Handler
.global PendSV_Handler
.global SysTick_Handler

@ external linker variables
.extern _stack_top
.extern _sidata
.extern _sdata
.extern _edata
.extern _sbss
.extern _ebss

.extern main

VectorTable:
    .word _stack_top
    .word Reset_Handler
    .word NMI_Handler
    .word HardFault_Handler
    .word MemManage_Handler
    .word BusFault_Handler
    .word UsageFault_Handler

    .word 0
    .word 0
    .word 0
    .word 0

    .word SVC_Handler

    .word 0
    .word 0

    .word PendSV_Handler
    .word SysTick_Handler



/******************************************************************************
 * Startup Code Section
 ******************************************************************************/
.section .text.Reset_Handler, "ax", %progbits

/******************************************************************************
 * Reset Handler
 ******************************************************************************/

.thumb_func
Reset_Handler:

    ldr r0, =_sidata
    ldr r1, =_sdata
    ldr r2, =_edata

CopyData:
    cmp r1, r2
    bge CopyDataDone

    ldr r3, [r0]
    str r3, [r1]

    add r0, r0, #4
    add r1, r1, #4

    b CopyData

CopyDataDone:
    ldr r1, =_sbss
    ldr r2, =_ebss


ZeroBss:
    cmp r1, r2
    bge ZeroBssDone

    movs r3, #0
    str r3, [r1]

    add r1, r1, #4
    b ZeroBss

ZeroBssDone:
    bl main

@ if return from main
Hang:
    b Hang


.thumb_func
NMI_Handler:
    b Default_Handler

.thumb_func
HardFault_Handler:
    b Default_Handler

.thumb_func
MemManage_Handler:
    b Default_Handler

.thumb_func
BusFault_Handler:
    b Default_Handler

.thumb_func
UsageFault_Handler:
    b Default_Handler

.thumb_func
SVC_Handler:
    b Default_Handler

.thumb_func
PendSV_Handler:
    b Default_Handler

.thumb_func
SysTick_Handler:
    b Default_Handler


/******************************************************************************
 * Default Exception Handler
 ******************************************************************************/

.thumb_func
Default_Handler:
Loop:
    b Loop