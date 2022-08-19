/*
 * Copyright 2020-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <cstdio>
#include "erpc_usb_cdc_transport.hpp"
#include "ns-usb.h"

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

static volatile bool s_isTransferReceiveCompleted = false;
static volatile bool s_isTransferSendCompleted = false;
static UsbCdcTransport *s_usbcdc_instance = NULL;

// SDK_ALIGN(static uint8_t s_serialWriteHandleBuffer[SERIAL_MANAGER_WRITE_HANDLE_SIZE], 4);
// static serial_write_handle_t s_serialWriteHandle = &s_serialWriteHandleBuffer[0]; /*!< serial manager write handle */

// SDK_ALIGN(static uint8_t s_serialReadHandleBuffer[SERIAL_MANAGER_READ_HANDLE_SIZE], 4);
// static serial_read_handle_t s_serialReadHandle = &s_serialReadHandleBuffer[0]; /*!< serial manager read handle */
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


static void ERPC_ns_usb_tx_cb(ns_usb_transaction_t *tr)
{
    UsbCdcTransport *transport = s_usbcdc_instance;
    if (NULL != tr)
    {
        if (AM_HAL_STATUS_SUCCESS == tr->status)
        {
            transport->tx_cb();
        }
        else
        {
            /* Handle other status if needed */
        }
    }
}

static void ERPC_ns_usb_rx_cb(ns_usb_transaction_t *tr)
{
    UsbCdcTransport *transport = s_usbcdc_instance;
    if (NULL != tr)
    {
        if (AM_HAL_STATUS_SUCCESS == tr->status)
        {
            transport->rx_cb();
        }
        else
        {
            /* Handle other status if needed */
        }
    }
}


void UsbCdcTransport::tx_cb(void)
{
#if !ERPC_THREADS_IS(NONE)
    m_txSemaphore.putFromISR();
#else
    s_isTransferSendCompleted = true;
#endif
}

void UsbCdcTransport::rx_cb(void)
{
#if !ERPC_THREADS_IS(NONE)
    m_rxSemaphore.putFromISR();
#else
    s_isTransferReceiveCompleted = true;
#endif
}

UsbCdcTransport::UsbCdcTransport(usb_handle_t usbHandle)
: m_usbHandle(usbHandle)
#if !ERPC_THREADS_IS(NONE)
, m_rxSemaphore()
, m_txSemaphore()
#endif
{
    s_usbcdc_instance = this;
}

UsbCdcTransport::~UsbCdcTransport(void)
{
    /* Cleanup */
    // SerialManager_CloseWriteHandle(s_serialWriteHandle);
    // SerialManager_CloseReadHandle(s_serialReadHandle);
    // SerialManager_Deinit(m_serialHandle);
}

erpc_status_t UsbCdcTransport::init(void)
{
    erpc_status_t status = kErpcStatus_InitFailed;

    ns_usb_register_callbacks(m_usbHandle, ERPC_ns_usb_rx_cb, ERPC_ns_usb_tx_cb);

    return status;
}

erpc_status_t UsbCdcTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
    erpc_status_t status = kErpcStatus_ReceiveFailed;

    s_isTransferReceiveCompleted = false;

    uint32_t bytes_rx = ns_usb_recieve_data(m_usbHandle, data, size); // blocking
    return status;
}
//      {


//     if (kStatus_SerialManager_Success == SerialManager_ReadNonBlocking(s_serialReadHandle, data, size))
//     {
// /* wait until the receiving is finished */
// #if !ERPC_THREADS_IS(NONE)
//         (void)m_rxSemaphore.get();
// #else
//         while (!s_isTransferReceiveCompleted)
//         {
//         }
// #endif
//         status = kErpcStatus_Success;
//     }

//     return status;
// }

erpc_status_t UsbCdcTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
    erpc_status_t status = kErpcStatus_SendFailed;

    s_isTransferSendCompleted = false;


    uint32_t bytes_rx = ns_usb_send_data(m_usbHandle, (uint8_t*)data, size); // blocking
    return status;

//     if (kStatus_SerialManager_Success == SerialManager_WriteNonBlocking(s_serialWriteHandle, data, size))
//     {
// /* wait until the sending is finished */
// #if !ERPC_THREADS_IS(NONE)
//         (void)m_txSemaphore.get();
// #else
//         while (!s_isTransferSendCompleted)
//         {
//         }
// #endif
//         status = kErpcStatus_Success;
//     }

//     return status;
}
