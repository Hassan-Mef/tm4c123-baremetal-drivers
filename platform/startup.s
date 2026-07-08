/* Thumb and ARM both will be used */
.syntax unified

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

/* Define Reset_Handler */
.section .text.Reset_Handler
.weak Reset_Handler
.type Reset_Handler, %function

Reset_Handler:
    /* CPACR is located at address 0xE000ED88 */
    ldr R0, =0xE000ED88
    /* Read CPACR */
    ldr R1, [R0]
    /* Set bits 20-23 to enable CP10 and CP11 coprocessors */
    orr R1, R1, #(0xF << 20)
    /* Write back the modified value to the CPACR */
    str R1, [R0] /* wait for store to complete */
    dsb
    /* reset pipeline now the FPU is enabled */
    isb

/* Copy the data segment initializers from flash to SRAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

/* Zero fill the bss segment. */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

FillZerobss:
    str  r3, [r2]
    adds r2, r2, #4

LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

/* Branch and link to main */
    bl main

.size Reset_Handler, .-Reset_Handler

/* Define Default_Handler (an infinite loop in case of exceptions) */
.section .text.Default_Handler

Default_Handler:
    Infinite_Loop:
        b Infinite_Loop

.size Default_Handler, .-Default_Handler

/* Define vector table */
.section .isr_vector
.type g_pfnVectors, %object
.size g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
    .word _estack
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
    .word DebugMon_Handler
    .word 0
    .word PendSV_Handler
    .word SysTick_Handler

    .word GPIOA_IRQHandler
    .word GPIOB_IRQHandler
    .word GPIOC_IRQHandler
    .word GPIOD_IRQHandler
    .word GPIOE_IRQHandler
    .word UART0_IRQHandler
    .word UART1_IRQHandler
    .word SSI0_IRQHandler
    .word I2C0_IRQHandler
    .word PWM0_FAULT_IRQHandler
    .word PWM0_GEN0_IRQHandler
    .word PWM0_GEN1_IRQHandler
    .word PWM0_GEN2_IRQHandler
    .word QEI0_IRQHandler
    .word ADC0_SEQUENCE0_IRQHandler
    .word ADC0_SEQUENCE1_IRQHandler
    .word ADC0_SEQUENCE2_IRQHandler
    .word ADC0_SEQUENCE3_IRQHandler
    .word WWDG_TIM01_IRQHandler
    .word TIM0A_IRQHandler
    .word TIM0B_IRQHandler
    .word TIM1A_IRQHandler
    .word TIM1B_IRQHandler
    .word TIM2A_IRQHandler
    .word TIM2B_IRQHandler
    .word ANALOG_COMP0_IRQHandler
    .word ANALOG_COMP1_IRQHandler
    .word 0
    .word SYS_CTRL_IRQHandler
    .word FMC_EEPROMC_IRQHandler
    .word GPIOF_IRQHandler
    .word 0
    .word 0
    .word UART2_IRQHandler
    .word SSI1_IRQHandler
    .word TIM3A_IRQHandler
    .word TIM3B_IRQHandler
    .word I2C1_IRQHandler
    .word QEI1_IRQHandler
    .word CAN0_IRQHandler
    .word CAN1_IRQHandler
    .word 0
    .word 0
    .word HIBM_IRQHandler
    .word USB_IRQHandler
    .word PWM0_GEN3_IRQHandler
    .word DMA_SOFT_IRQHandler
    .word DMA_ERR_IRQHandler
    .word ADC1_SEQUENCE0_IRQHandler
    .word ADC1_SEQUENCE1_IRQHandler
    .word ADC1_SEQUENCE2_IRQHandler
    .word ADC1_SEQUENCE3_IRQHandler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SSI2_IRQHandler
    .word SSI3_IRQHandler
    .word UART3_IRQHandler
    .word UART4_IRQHandler
    .word UART5_IRQHandler
    .word UART6_IRQHandler
    .word UART7_IRQHandler
    .word 0
    .word 0
    .word 0
    .word 0
    .word I2C2_IRQHandler
    .word I2C3_IRQHandler
    .word TIM4A_IRQHandler
    .word TIM4B_IRQHandler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word TIM5A_IRQHandler
    .word TIM5B_IRQHandler
    .word WIDE_TIM0A_IRQHandler
    .word WIDE_TIM0B_IRQHandler
    .word WIDE_TIM1A_IRQHandler
    .word WIDE_TIM1B_IRQHandler
    .word WIDE_TIM2A_IRQHandler
    .word WIDE_TIM2B_IRQHandler
    .word WIDE_TIM3A_IRQHandler
    .word WIDE_TIM3B_IRQHandler
    .word WIDE_TIM4A_IRQHandler
    .word WIDE_TIM4B_IRQHandler
    .word WIDE_TIM5A_IRQHandler
    .word WIDE_TIM5B_IRQHandler
    .word SYS_EXP_IRQHandler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word PWM1_GEN0_IRQHandler
    .word PWM1_GEN1_IRQHandler
    .word PWM1_GEN2_IRQHandler
    .word PWM1_GEN3_IRQHandler
    .word PWM1_FAULT_IRQHandler

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

    .weak NMI_Handler
    .thumb_set NMI_Handler,Default_Handler

    .weak HardFault_Handler
    .thumb_set HardFault_Handler,Default_Handler

    .weak MemManage_Handler
    .thumb_set MemManage_Handler,Default_Handler

    .weak BusFault_Handler
    .thumb_set BusFault_Handler,Default_Handler

    .weak UsageFault_Handler
    .thumb_set UsageFault_Handler,Default_Handler

    .weak SVC_Handler
    .thumb_set SVC_Handler,Default_Handler

    .weak DebugMon_Handler
    .thumb_set DebugMon_Handler,Default_Handler

    .weak PendSV_Handler
    .thumb_set PendSV_Handler,Default_Handler

    .weak SysTick_Handler
    .thumb_set SysTick_Handler,Default_Handler



    .weak GPIOA_IRQHandler
    .thumb_set GPIOA_IRQHandler, Default_Handler

    .weak GPIOB_IRQHandler
    .thumb_set GPIOB_IRQHandler, Default_Handler

    .weak GPIOC_IRQHandler
    .thumb_set GPIOC_IRQHandler, Default_Handler

    .weak GPIOD_IRQHandler
    .thumb_set GPIOD_IRQHandler, Default_Handler

    .weak GPIOE_IRQHandler
    .thumb_set GPIOE_IRQHandler, Default_Handler

    .weak UART0_IRQHandler
    .thumb_set UART0_IRQHandler, Default_Handler

    .weak UART1_IRQHandler
    .thumb_set UART1_IRQHandler, Default_Handler

    .weak SSI0_IRQHandler
    .thumb_set SSI0_IRQHandler, Default_Handler

    .weak I2C0_IRQHandler
    .thumb_set I2C0_IRQHandler, Default_Handler

    .weak PWM0_FAULT_IRQHandler
    .thumb_set PWM0_FAULT_IRQHandler, Default_Handler

    .weak PWM0_GEN0_IRQHandler
    .thumb_set PWM0_GEN0_IRQHandler, Default_Handler

    .weak PWM0_GEN1_IRQHandler
    .thumb_set PWM0_GEN1_IRQHandler, Default_Handler

    .weak PWM0_GEN2_IRQHandler
    .thumb_set PWM0_GEN2_IRQHandler, Default_Handler

    .weak QEI0_IRQHandler
    .thumb_set QEI0_IRQHandler, Default_Handler

    .weak ADC0_SEQUENCE0_IRQHandler
    .thumb_set ADC0_SEQUENCE0_IRQHandler, Default_Handler

    .weak ADC0_SEQUENCE1_IRQHandler
    .thumb_set ADC0_SEQUENCE1_IRQHandler, Default_Handler

    .weak ADC0_SEQUENCE2_IRQHandler
    .thumb_set ADC0_SEQUENCE2_IRQHandler, Default_Handler

    .weak ADC0_SEQUENCE3_IRQHandler
    .thumb_set ADC0_SEQUENCE3_IRQHandler, Default_Handler

    .weak WWDG_TIM01_IRQHandler
    .thumb_set WWDG_TIM01_IRQHandler, Default_Handler

    .weak TIM0A_IRQHandler
    .thumb_set TIM0A_IRQHandler, Default_Handler

    .weak TIM0B_IRQHandler
    .thumb_set TIM0B_IRQHandler, Default_Handler

    .weak TIM1A_IRQHandler
    .thumb_set TIM1A_IRQHandler, Default_Handler

    .weak TIM1B_IRQHandler
    .thumb_set TIM1B_IRQHandler, Default_Handler

    .weak TIM2A_IRQHandler
    .thumb_set TIM2A_IRQHandler, Default_Handler

    .weak TIM2B_IRQHandler
    .thumb_set TIM2B_IRQHandler, Default_Handler

    .weak ANALOG_COMP0_IRQHandler
    .thumb_set ANALOG_COMP0_IRQHandler, Default_Handler

    .weak ANALOG_COMP1_IRQHandler
    .thumb_set ANALOG_COMP1_IRQHandler, Default_Handler

    .weak SYS_CTRL_IRQHandler
    .thumb_set SYS_CTRL_IRQHandler, Default_Handler

    .weak FMC_EEPROMC_IRQHandler
    .thumb_set FMC_EEPROMC_IRQHandler, Default_Handler

    .weak GPIOF_IRQHandler
    .thumb_set GPIOF_IRQHandler, Default_Handler

    .weak UART2_IRQHandler
    .thumb_set UART2_IRQHandler, Default_Handler

    .weak SSI1_IRQHandler
    .thumb_set SSI1_IRQHandler, Default_Handler

    .weak TIM3A_IRQHandler
    .thumb_set TIM3A_IRQHandler, Default_Handler

    .weak TIM3B_IRQHandler
    .thumb_set TIM3B_IRQHandler, Default_Handler

    .weak I2C1_IRQHandler
    .thumb_set I2C1_IRQHandler, Default_Handler

    .weak QEI1_IRQHandler
    .thumb_set QEI1_IRQHandler, Default_Handler

    .weak CAN0_IRQHandler
    .thumb_set CAN0_IRQHandler, Default_Handler

    .weak CAN1_IRQHandler
    .thumb_set CAN1_IRQHandler, Default_Handler

    .weak HIBM_IRQHandler
    .thumb_set HIBM_IRQHandler, Default_Handler

    .weak USB_IRQHandler
    .thumb_set USB_IRQHandler, Default_Handler

    .weak PWM0_GEN3_IRQHandler
    .thumb_set PWM0_GEN3_IRQHandler, Default_Handler

    .weak DMA_SOFT_IRQHandler
    .thumb_set DMA_SOFT_IRQHandler, Default_Handler

    .weak DMA_ERR_IRQHandler
    .thumb_set DMA_ERR_IRQHandler, Default_Handler

    .weak ADC1_SEQUENCE0_IRQHandler
    .thumb_set ADC1_SEQUENCE0_IRQHandler, Default_Handler

    .weak ADC1_SEQUENCE1_IRQHandler
    .thumb_set ADC1_SEQUENCE1_IRQHandler, Default_Handler

    .weak ADC1_SEQUENCE2_IRQHandler
    .thumb_set ADC1_SEQUENCE2_IRQHandler, Default_Handler

    .weak ADC1_SEQUENCE3_IRQHandler
    .thumb_set ADC1_SEQUENCE3_IRQHandler, Default_Handler

    .weak SSI2_IRQHandler
    .thumb_set SSI2_IRQHandler, Default_Handler

    .weak SSI3_IRQHandler
    .thumb_set SSI3_IRQHandler, Default_Handler

    .weak UART3_IRQHandler
    .thumb_set UART3_IRQHandler, Default_Handler

    .weak UART4_IRQHandler
    .thumb_set UART4_IRQHandler, Default_Handler

    .weak UART5_IRQHandler
    .thumb_set UART5_IRQHandler, Default_Handler

    .weak UART6_IRQHandler
    .thumb_set UART6_IRQHandler, Default_Handler

    .weak UART7_IRQHandler
    .thumb_set UART7_IRQHandler, Default_Handler

    .weak I2C2_IRQHandler
    .thumb_set I2C2_IRQHandler, Default_Handler

    .weak I2C3_IRQHandler
    .thumb_set I2C3_IRQHandler, Default_Handler

    .weak TIM4A_IRQHandler
    .thumb_set TIM4A_IRQHandler, Default_Handler

    .weak TIM4B_IRQHandler
    .thumb_set TIM4B_IRQHandler, Default_Handler

    .weak TIM5A_IRQHandler
    .thumb_set TIM5A_IRQHandler, Default_Handler

    .weak TIM5B_IRQHandler
    .thumb_set TIM5B_IRQHandler, Default_Handler

    .weak WIDE_TIM0A_IRQHandler
    .thumb_set WIDE_TIM0A_IRQHandler, Default_Handler

    .weak WIDE_TIM0B_IRQHandler
    .thumb_set WIDE_TIM0B_IRQHandler, Default_Handler

    .weak WIDE_TIM1A_IRQHandler
    .thumb_set WIDE_TIM1A_IRQHandler, Default_Handler

    .weak WIDE_TIM1B_IRQHandler
    .thumb_set WIDE_TIM1B_IRQHandler, Default_Handler

    .weak WIDE_TIM2A_IRQHandler
    .thumb_set WIDE_TIM2A_IRQHandler, Default_Handler

    .weak WIDE_TIM2B_IRQHandler
    .thumb_set WIDE_TIM2B_IRQHandler, Default_Handler

    .weak WIDE_TIM3A_IRQHandler
    .thumb_set WIDE_TIM3A_IRQHandler, Default_Handler

    .weak WIDE_TIM3B_IRQHandler
    .thumb_set WIDE_TIM3B_IRQHandler, Default_Handler

    .weak WIDE_TIM4A_IRQHandler
    .thumb_set WIDE_TIM4A_IRQHandler, Default_Handler

    .weak WIDE_TIM4B_IRQHandler
    .thumb_set WIDE_TIM4B_IRQHandler, Default_Handler

    .weak WIDE_TIM5A_IRQHandler
    .thumb_set WIDE_TIM5A_IRQHandler, Default_Handler

    .weak WIDE_TIM5B_IRQHandler
    .thumb_set WIDE_TIM5B_IRQHandler, Default_Handler

    .weak SYS_EXP_IRQHandler
    .thumb_set SYS_EXP_IRQHandler, Default_Handler

    .weak PWM1_GEN0_IRQHandler
    .thumb_set PWM1_GEN0_IRQHandler, Default_Handler

    .weak PWM1_GEN1_IRQHandler
    .thumb_set PWM1_GEN1_IRQHandler, Default_Handler

    .weak PWM1_GEN2_IRQHandler
    .thumb_set PWM1_GEN2_IRQHandler, Default_Handler

    .weak PWM1_GEN3_IRQHandler
    .thumb_set PWM1_GEN3_IRQHandler, Default_Handler

    .weak PWM1_FAULT_IRQHandler
    .thumb_set PWM1_FAULT_IRQHandler, Default_Handler

/* End of file */
