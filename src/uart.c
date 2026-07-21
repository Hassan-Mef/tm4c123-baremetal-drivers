/**************************************************************************************************
 * FILENAME : uart.c
 * DESCRIPTION : Contains UART driver function definitions.
 *
 * NOTES :
 *
 * AUTHOR : Hassan
 ***************************************************************************************************/

/*************************************** Header Inclusion*****************************************/
#include "uart.h"
#include "gpio.h"

/********************************************* Globals ********************************************/

static uart_registerType *const uartRegisters[] =
{
    UART0,
    UART1,
    UART2,
    UART3,
    UART4,
    UART5,
    UART6,
    UART7
};

static const uint32_t uartClockMask[] =
{
    (1U << 0),
    (1U << 1),
    (1U << 2),
    (1U << 3),
    (1U << 4),
    (1U << 5),
    (1U << 6),
    (1U << 7)
};

typedef struct
{
    gpio_portType port;
    gpio_pinType txPin;
    gpio_pinType rxPin;

} uart_pinConfigType;

static const uart_pinConfigType uartPins[] =
{
    /* UART0 */
    {GPIO_PORT_A, GPIO_PIN_1, GPIO_PIN_0},

    /* UART1 */
    {GPIO_PORT_B, GPIO_PIN_1, GPIO_PIN_0},

     /* UART2 */
    {GPIO_PORT_D, GPIO_PIN_7, GPIO_PIN_6},

    /* UART3 */
    {GPIO_PORT_C, GPIO_PIN_7, GPIO_PIN_6},

    /* UART4 */
    {GPIO_PORT_C, GPIO_PIN_5, GPIO_PIN_4},

    /* UART5 */
    {GPIO_PORT_E, GPIO_PIN_5, GPIO_PIN_4},

    /* UART6 */
    {GPIO_PORT_D, GPIO_PIN_5, GPIO_PIN_4},

    /* UART7 */
    {GPIO_PORT_E, GPIO_PIN_1, GPIO_PIN_0}
};

/* IRQ Lookup Table */
static const uart_irqNumberType uartIRQTable[] =
{
    UART0_IRQ,
    UART1_IRQ,
    UART2_IRQ,
    UART3_IRQ,
    UART4_IRQ,
    UART5_IRQ,
    UART6_IRQ,
    UART7_IRQ
};

/* Last received character for each UART */
static volatile char uartReceivedData[UART_INVALID];

static void uart_rxEchoCallback(void);
/* UART RX callback table */
static void (*uartCallbacks[UART_INVALID])(void) = 
{
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback, 
    uart_rxEchoCallback,
};

static uart_numberType currentInterruptUART = UART_INVALID;

/************************************* Function Implementations***********************************/

/**
 * @brief uart_setBaudRate : Configures the UART baud rate.
 *
 * Calculates and programs the integer and fractional baud-rate
 * divisors for the selected UART peripheral.
 *
 * @param uart : Pointer to UART register structure.
 * @param baudRate : Desired baud rate.
 *
 * @return void
 */
static void uart_setBaudRate(uart_registerType *uart, uint32_t baudRate)
{
    uint32_t divisor;
    uint32_t integer;
    uint32_t fractional;

    divisor = 16U * baudRate;

    integer = clock_getSystemFrequency() / divisor;

    fractional = ((clock_getSystemFrequency() % divisor) * 64U +
                  (divisor / 2U)) /
                 divisor;

    uart->UARTIBRD = integer;
    uart->UARTFBRD = fractional;
}

/**
 * @brief uart_configurePins : Configures GPIO pins for the selected UART.
 *
 * @param uartNumber : UART peripheral number.
 *
 * @return uart_errorType
 */
static uart_errorType uart_configurePins(uart_numberType uartNumber)
{
    gpio_configType gpioConfig;
    gpio_registersType *gpio = NULL;
    const uart_pinConfigType *pins = NULL;

    if (uartNumber >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    /* Get selected UART pin configuration */
    pins = &uartPins[uartNumber];

    /* Configure RX pin */
    gpioConfig.port = pins->port;
    gpioConfig.pin = pins->rxPin;
    gpioConfig.mode = GPIO_MODE_ALTERNATE;

    if (gpio_init(&gpioConfig) != GPIO_SUCCESS)
    {
        return UART_INVALID_CONFIG;
    }

    /* Configure TX pin */
    gpioConfig.pin = pins->txPin;

    if (gpio_init(&gpioConfig) != GPIO_SUCCESS)
    {
        return UART_INVALID_CONFIG;
    }

    /* Get GPIO register pointer */
    switch (pins->port)
    {
    case GPIO_PORT_A:
        gpio = GPIOA;
        break;

    case GPIO_PORT_B:
        gpio = GPIOB;
        break;

    case GPIO_PORT_C:
        gpio = GPIOC;
        break;

    case GPIO_PORT_D:
        gpio = GPIOD;
        break;

    case GPIO_PORT_E:
        gpio = GPIOE;
        break;

    case GPIO_PORT_F:
        gpio = GPIOF;
        break;

    default:
        return UART_INVALID_CONFIG;
    }

    /* Disable analog function */
    gpio->AMSEL &= ~(1U << pins->rxPin);
    gpio->AMSEL &= ~(1U << pins->txPin);

    /* Configure GPIO Port Control for UART alternate function */

    /* RX */
    gpio->PCTL &= ~(0xFU << (pins->rxPin * GPIO_PCTL_PIN_OFFSET));
    gpio->PCTL |= (GPIO_PCTL_UART << (pins->rxPin * GPIO_PCTL_PIN_OFFSET));

    /* TX */
    gpio->PCTL &= ~(0xFU << (pins->txPin * GPIO_PCTL_PIN_OFFSET));
    gpio->PCTL |= (GPIO_PCTL_UART << (pins->txPin * GPIO_PCTL_PIN_OFFSET));

    return UART_SUCCESS;
}

/**
 * @brief uart_init : Initializes the selected UART peripheral.
 *
 * Configures the UART baud rate, frame format, GPIO pins,
 * clock source, and optionally enables receive interrupts.
 *
 * @param config : Pointer to UART configuration.
 *
 * @return uart_errorType
 */
uart_errorType uart_init(uart_configType* config)
{

    uart_registerType *uart = NULL;

    /* Validate input pointer */
    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* Validate UART number */
    if (config->number >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    /* Validate baud rate */
    if (config->baudRate == 0U)
    {
        return UART_INVALID_BAUDRATE;
    }

    /* Enable UART module clock */
    SYSCTL_RCGCUART |= uartClockMask[config->number];

    /* Configure UART GPIO pins */
    if (uart_configurePins(config->number) != UART_SUCCESS)
    {
        return UART_INVALID_CONFIG;
    }

    /* Get UART register pointer */
    uart = uartRegisters[config->number];

    /* Disable UART before configuration */
    uart->UARTCTL &= ~(1U << UARTCTL_UARTEN_BIT);

    /* Configure baud rate */
    uart_setBaudRate(uart, config->baudRate);

    /* Configure frame format
     *  - 8 data bits
     *  - No parity
     *  - One stop bit
     */
    uart->UARTLCRH =
    (config->wordLength << UARTLCRH_WLEN_BIT);

    switch (config->parity)
    {
    case UART_PARITY_NONE:
        uart->UARTLCRH &= ~(1U << UARTLCRH_PEN_BIT);
        break;

    case UART_PARITY_EVEN:
        uart->UARTLCRH |= (1U << UARTLCRH_PEN_BIT);
        uart->UARTLCRH |= (1U << UARTLCRH_EPS_BIT);
        break;

    case UART_PARITY_ODD:
        uart->UARTLCRH |= (1U << UARTLCRH_PEN_BIT);
        uart->UARTLCRH &= ~(1U << UARTLCRH_EPS_BIT);
        break;
    default:
        return  UART_INVALID_CONFIG;
    }


    if (config->stopBits == UART_STOP_BITS_2)
    {
        uart->UARTLCRH |= (1U << UARTLCRH_STP2_BIT);
    }
    else
    {
        uart->UARTLCRH &= ~(1U << UARTLCRH_STP2_BIT);
    }

    /* Select System Clock as UART clock source */
    uart->UARTCC = 0U;

    /* Enable transmitter and receiver */
    uart->UARTCTL |=
        (1U << UARTCTL_TXE_BIT) |
        (1U << UARTCTL_RXE_BIT);

    /* Enable UART interrupt if requested */
    if (config->interruptEnable)
    {
        /* Enable RX interrupt in UARTIM register */
        uart->UARTIM |= (1U << UARTIM_RXIM_BIT);

        /* Enable UART interrupt in NVIC */
        uint32_t irqNumber = uartIRQTable[config->number];
        NVIC_ENABLE_BASE[irqNumber / IRQ_REGISTER_DIVISION_FACTOR] |= (1U << (irqNumber % IRQ_REGISTER_DIVISION_FACTOR));
    }

    /* Enable UART */
    uart->UARTCTL |= (1U << UARTCTL_UARTEN_BIT);

    return UART_SUCCESS;
}

/**
 * @brief uart_deInit : Deinitializes the selected UART peripheral.
 * Disables the UART transmitter, receiver, UART interrupts,
 * and the corresponding NVIC interrupt.
 *
 * @param config : Pointer to UART configuration.
 *
 * @return uart_errorType
 */
uart_errorType uart_deInit(uart_configType* config)
{
    uart_registerType *uart = NULL;
    uint32_t irqNumber;

    /* Validate pointer */
    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* Validate UART number */
    if (config->number >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    uart = uartRegisters[config->number];

    /* Disable UART interrupts */
    uart->UARTIM = 0U;

    /* Clear pending UART interrupts */
    uart->UARTICR = 0xFFFFFFFFU;

    /* Disable NVIC interrupt */
    irqNumber = uartIRQTable[config->number];

    /* NVIC ICER registers start at 0xE000E180 */
    volatile uint32_t *NVIC_DISABLE_BASE =
        (volatile uint32_t *)0xE000E180U;

    NVIC_DISABLE_BASE[irqNumber / IRQ_REGISTER_DIVISION_FACTOR] =
        (1U << (irqNumber % IRQ_REGISTER_DIVISION_FACTOR));

    /* Disable transmitter */
    uart->UARTCTL &= ~(1U << UARTCTL_TXE_BIT);

    /* Disable receiver */
    uart->UARTCTL &= ~(1U << UARTCTL_RXE_BIT);

    /* Disable UART module */
    uart->UARTCTL &= ~(1U << UARTCTL_UARTEN_BIT);

    return UART_SUCCESS;
}

/**
 * @brief uart_changeBuadRate : Chnages the cureent Baud  
 *
 * @param config : Pointer to UART configuration.
 *
 * @return uart_errorType
 */
uart_errorType uart_changeBaudRate(uart_configType *config,uint32_t baudRate)
{
    uart_registerType *uart  = uartRegisters[config->number];

    while (uart->UARTFR & (1U << UARTFR_BUSY_BIT))
    {
    }

    /* disable Uart*/
    uart->UARTCTL &= ~(1U << UARTCTL_UARTEN_BIT);

    uart_setBaudRate(uart, baudRate);

    uart->UARTLCRH = uart->UARTLCRH;

    uart->UARTCTL |= (1U << UARTCTL_UARTEN_BIT);

    return UART_SUCCESS;
    
}

/**
 * @brief uart_sendCharacter : Sends a single character over UART.
 *
 * @param config : Pointer to UART configuration.
 * @param data   : Character to transmit.
 *
 * @return uart_errorType
 */
uart_errorType uart_sendCharacter(uart_configType* config, char data)
{
    uart_registerType *uart = NULL;

    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (config->number >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    uart = uartRegisters[config->number];

      /* Convert LF to CR+LF */
    if (data == '\n')
    {
        while (uart->UARTFR & (1U << UARTFR_TXFF_BIT))
        {
        }

        uart->UARTDR = '\r';
    }

    /* Wait until TX FIFO is not full */
    while (uart->UARTFR & (1U << UARTFR_TXFF_BIT))    
    {
    }

    /* Transmit character */
    uart->UARTDR = (uint32_t)data;

    return UART_SUCCESS;
}

/**
 * @brief uart_sendString : Sends a NULL terminated string.
 *
 * @param config : Pointer to UART configuration.
 * @param data   : String to transmit.
 *
 * @return uart_errorType
 */
uart_errorType uart_sendString(uart_configType* config, char* data)
{
    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (data == NULL)
    {
        return UART_NULL_POINTER;
    }

    while (*data != '\0')
    {
        uart_sendCharacter(config, *data);
        data++;
    }

    return UART_SUCCESS;
}

/**
 * @brief uart_receiveCharacter : Receives a character.
 *
 * Waits until a character is available in the receive buffer,
 * then reads and returns it.
 *
 * @param config : Pointer to UART configuration.
 * @param data : Pointer to store the received character.
 *
 * @return uart_errorType
 */
uart_errorType uart_receiveCharacter(uart_configType* config, char* data)
{
    uart_registerType *uart = NULL;

    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (data == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (config->number >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    uart = uartRegisters[config->number];

    /* Wait until data is available */
    while (uart->UARTFR & (1U << UARTFR_RXFE_BIT))
    {
    }

    /* Read received character */
    *data = (char)(uart->UARTDR & 0xFFU);

    

    return UART_SUCCESS;
}

/**
 * @brief uart_receiveNB : Receives a character without blocking.
 *
 * Checks whether a character is available in the receive buffer.
 * If data is available, the received character is returned
 * immediately. Otherwise, the function returns UART_BUSY.
 *
 * @param config : Pointer to UART configuration.
 * @param data : Pointer to store the received character.
 *
 * @return uart_errorType
 */
uart_errorType uart_receiveNB(uart_configType* config, char* data)
{
    uart_registerType *uart = NULL;

    if (config == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (data == NULL)
    {
        return UART_NULL_POINTER;
    }

    if (config->number >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    uart = uartRegisters[config->number];

    /* No data available */
    if (uart->UARTFR & (1U << UARTFR_RXFE_BIT))
    {
        return UART_BUSY;
    }

    *data = (char)(uart->UARTDR & 0xFFU);

    return UART_SUCCESS;
}

/**
 * @brief uart_getReceivedCharacter : Retrieves the last received UART character.
 * Returns the character stored by the UART interrupt handler for
 * the selected UART peripheral.
 *
 * @param config : Pointer to UART configuration.  
 * @param data : Pointer to store the received character.
 *
 * @return uart_errorType
 */
uart_errorType uart_getReceivedCharacter(uart_configType* config, char* data)
{
    uint8_t uartNumber = config->number ;
    if (uartNumber >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    if (data == NULL)
    {
        return UART_NULL_POINTER;
    }

    *data = uartReceivedData[uartNumber];

    return UART_SUCCESS;
    
}

/**
 * @brief uart_interruptHandler : Handles UART receive interrupts.
 * Reads the received character, clears the receive interrupt flag,
 * stores the received data
 *
 * @param config : Pointer to UART configuration.
 *
 * @return uart_errorType
 */
uart_errorType uart_interruptHandler(uart_numberType uartNumber)
{
    uart_registerType *uart = NULL;

    if (uartNumber >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    currentInterruptUART = uartNumber;
    
    uart = uartRegisters[uartNumber];

    /* Read received character */
    uartReceivedData[uartNumber] = (char)(uart->UARTDR & 0xFFU);


    uart->UARTICR = (1U << UARTICR_RXIC_BIT);

    if(uartCallbacks[uartNumber] != NULL)
    {
        uartCallbacks[uartNumber]();
    }

    return UART_SUCCESS;
}

/**
 * @brief uart_setCallback : Registers a callback function for a UART.
 *
 * Associates a user-defined callback function with the selected
 * UART peripheral. The callback is executed whenever a receive
 * interrupt occurs.
 *
 * @param config : Pointer to UART configuration.
 * @param callback : Pointer to callback function.
 *
 * @return uart_errorType
 */
uart_errorType uart_setCallback(uart_configType* config, void (*callback)(void))
{
    uint8_t uartNumber = config->number ;

    if (uartNumber >= UART_INVALID)
    {
        return UART_INVALID_UART;
    }

    if (callback == NULL)
    {
        return UART_NULL_POINTER;
    }

    /* Register the callback function */
    uartCallbacks[uartNumber] = callback;

    return UART_SUCCESS;
    
}

/**
 * @brief uart_rxEchoCallback : Default UART receive callback.
 *
 * Retrieves the received character from the interrupt buffer and
 * echoes it back through the UART that generated the interrupt.
 *
 * @return void
 */
static void uart_rxEchoCallback(void)
{
    char ch;
    uart_configType config;

    config.number = currentInterruptUART;

    if (uart_getReceivedCharacter(&config, &ch) == UART_SUCCESS)
    {
        uart_sendCharacter(&config, ch);
    }
}
