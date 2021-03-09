/**
 * \file            lwesp_ll_stm32.c
 * \brief           Generic STM32 driver, included in various STM32 driver variants
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwESP - Lightweight ESP-AT parser library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         v1.0.0
 */

/*
 * How it works
 *
 * On first call to \ref lwesp_ll_init, new thread is created and processed in usart_ll_thread function.
 * USART is configured in RX DMA mode and any incoming bytes are processed inside thread function.
 * DMA and USART implement interrupt handlers to notify main thread about new data ready to send to upper layer.
 *
 * More about UART + RX DMA: https://github.com/MaJerle/stm32-usart-dma-rx-tx
 *
 * \ref LWESP_CFG_INPUT_USE_PROCESS must be enabled in `lwesp_config.h` to use this driver.
 */
#include "lwesp/lwesp.h"
#include "lwesp/lwesp_mem.h"
#include "lwesp/lwesp_input.h"
#include "system/lwesp_ll.h"

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#if !__DOXYGEN__

    #if !LWESP_CFG_INPUT_USE_PROCESS
        #error "LWESP_CFG_INPUT_USE_PROCESS must be enabled in `lwesp_config.h` to use this driver."
    #endif /* LWESP_CFG_INPUT_USE_PROCESS */

    #if !defined(LWESP_USART_DMA_RX_BUFF_SIZE)
        #define LWESP_USART_DMA_RX_BUFF_SIZE 0x1000
    #endif /* !defined(LWESP_USART_DMA_RX_BUFF_SIZE) */

    #if !defined(LWESP_MEM_SIZE)
        #define LWESP_MEM_SIZE 0x1000
    #endif /* !defined(LWESP_MEM_SIZE) */

extern UART_HandleTypeDef huart6;

/* USART memory */
static uint8_t usart_mem[LWESP_USART_DMA_RX_BUFF_SIZE];
static uint8_t is_running, initialized;
static size_t old_pos;

/* USART thread */
static void usart_ll_thread(void *arg);
static osThreadId_t usart_ll_thread_id;

/* Message queue */
static osMessageQueueId_t usart_ll_mbox_id;

/**
 * \brief           USART data processing
 */
static void
usart_ll_thread(void *arg) {
    size_t pos;

    LWESP_UNUSED(arg);

    while (1) {
        void *d;
        /* Wait for the event message from DMA or USART */
        // osMessageQueueGet(usart_ll_mbox_id, &d, NULL, osWaitForever);

        /* Read data */
    // #if defined(LWESP_USART_DMA_RX_STREAM)
        // pos = sizeof(usart_mem) - LL_DMA_GetDataLength(LWESP_USART_DMA, LWESP_USART_DMA_RX_STREAM);
    // #else
        // pos = sizeof(usart_mem) - LL_DMA_GetDataLength(LWESP_USART_DMA, LWESP_USART_DMA_RX_CH);
    // #endif [> defined(LWESP_USART_DMA_RX_STREAM) <]
        // if (pos != old_pos && is_running) {
            // if (pos > old_pos) {
                // lwesp_input_process(&usart_mem[old_pos], pos - old_pos);
            // } else {
                // lwesp_input_process(&usart_mem[old_pos], sizeof(usart_mem) - old_pos);
                // if (pos > 0) {
                    // lwesp_input_process(&usart_mem[0], pos);
                // }
            // }
            // old_pos = pos;
            // if (old_pos == sizeof(usart_mem)) {
                // old_pos = 0;
            // }
        // }
    }
}

/**
 * \brief           Configure UART using DMA for receive in double buffer mode and IDLE line detection
 */
static void
configure_uart(uint32_t baudrate) {
    if (!initialized) {

        /* Global pin configuration */
        // MX_GPIO_Init(); // -- main.cpp

        /* Configure USART pins */

        /* TX PIN */
        /* RX PIN */
        /* Configure UART */
        /* Enable USART interrupts and DMA request */
        // MX_USART6_UART_Init() // -- main.cpp
        // uartrxbuff_t // -- used for PUT before

        /* Enable USART interrupts */
        // USART6_IRQHandler() // -- stm32f4xx_it.c

        /* Configure DMA */
        is_running = 0;

        // -- DMA for ESP code here TODO
        // MX_DMA_Init() // -- main.cpp

        old_pos = 0;
        is_running = 1;

    } else {
        osDelay(10);
        HAL_UART_DeInit(&huart6);
        huart6.Init.BaudRate = baudrate;
        if (HAL_UART_Init(&huart6) != HAL_OK) {
            // Error_Handler();
        }
    }

    /* Create mbox and start thread */
    // if (usart_ll_mbox_id == NULL) {
        // usart_ll_mbox_id = osMessageQueueNew(10, sizeof(void *), NULL);
    // }
    // if (usart_ll_thread_id == NULL) {
        // const osThreadAttr_t attr = {
            // .stack_size = 1024
        // };
        // usart_ll_thread_id = osThreadNew(usart_ll_thread, usart_ll_mbox_id, &attr);
    // }
}

    #if defined(LWESP_RESET_PIN)
/**
 * \brief           Hardware reset callback
 */
static uint8_t
reset_device(uint8_t state) {
    if (state) { /* Activate reset line */
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    }
    return 1;
}
    #endif /* defined(LWESP_RESET_PIN) */

/**
 * \brief           Send data to ESP device
 * \param[in]       data: Pointer to data to send
 * \param[in]       len: Number of bytes to send
 * \return          Number of bytes sent
 */
static size_t
send_data(const void *data, size_t len) {
    const uint8_t *d = data;
    HAL_UART_Transmit(&huart6, (uint8_t *)d, len, 1000);
    return len;
}

/**
 * \brief           Callback function called from initialization process
 */
lwespr_t
lwesp_ll_init(lwesp_ll_t *ll) {
    #if !LWESP_CFG_MEM_CUSTOM
    static uint8_t memory[LWESP_MEM_SIZE];
    lwesp_mem_region_t mem_regions[] = {
        { memory, sizeof(memory) }
    };

    if (!initialized) {
        lwesp_mem_assignmemory(mem_regions, LWESP_ARRAYSIZE(mem_regions)); /* Assign memory for allocations */
    }
    #endif /* !LWESP_CFG_MEM_CUSTOM */

    if (!initialized) {
        ll->send_fn = send_data; /* Set callback function to send data */
    #if defined(LWESP_RESET_PIN)
        ll->reset_fn = reset_device; /* Set callback for hardware reset */
    #endif                           /* defined(LWESP_RESET_PIN) */
    }

    configure_uart(ll->uart.baudrate); /* Initialize UART for communication */
    initialized = 1;
    return lwespOK;
}

/**
 * \brief           Callback function to de-init low-level communication part
 */
lwespr_t
lwesp_ll_deinit(lwesp_ll_t *ll) {
    if (usart_ll_mbox_id != NULL) {
        osMessageQueueId_t tmp = usart_ll_mbox_id;
        usart_ll_mbox_id = NULL;
        osMessageQueueDelete(tmp);
    }
    if (usart_ll_thread_id != NULL) {
        osThreadId_t tmp = usart_ll_thread_id;
        usart_ll_thread_id = NULL;
        osThreadTerminate(tmp);
    }
    initialized = 0;
    LWESP_UNUSED(ll);
    return lwespOK;
}

/**
 * \brief           UART global interrupt handler
 */
void LWESP_USART_IRQHANDLER(void) {
    // LL_USART_ClearFlag_IDLE(LWESP_USART);
    // LL_USART_ClearFlag_PE(LWESP_USART);
    // LL_USART_ClearFlag_FE(LWESP_USART);
    // LL_USART_ClearFlag_ORE(LWESP_USART);
    // LL_USART_ClearFlag_NE(LWESP_USART);
//
    // if (usart_ll_mbox_id != NULL) {
        // void *d = (void *)1;
        // osMessageQueuePut(usart_ll_mbox_id, &d, 0, 0);
    // }
}

/**
 * \brief           UART DMA stream/channel handler
 */
void LWESP_USART_DMA_RX_IRQHANDLER(void) {
    // LWESP_USART_DMA_RX_CLEAR_TC;
    // LWESP_USART_DMA_RX_CLEAR_HT;
//
    // if (usart_ll_mbox_id != NULL) {
        // void *d = (void *)1;
        // osMessageQueuePut(usart_ll_mbox_id, &d, 0, 0);
    // }
}

#endif /* !__DOXYGEN__ */
