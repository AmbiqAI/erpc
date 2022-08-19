# Porting Notes
1. Modified port_stdlib to use ns_malloc
2. Modified erpc_config.h to enable static allocation and no_threads
3. Modify setup_usb_cdc to init USB not

Example client main:
#include "erpc_matrix_multiply.h"
#include "erpc_client_setup.h"
#include "fsl_uart_cmsis.h" /* Not an eRPC file. Needs to be replaced with the included transport type. */

```c
int main()
{
    /* Matrices definitions */
    Matrix matrix1, matrix2, result_matrix = {{0}};

    /* Init eRPC client environment */
    /* UART transport layer initialization */
    erpc_transport_t transport = erpc_transport_cmsis_uart_init((void *)&DEMO_UART); /* DEMO_UART defined in fsl_uart_cmsis.h */

    /* MessageBufferFactory initialization */
    erpc_mbf_t message_buffer_factory = erpc_mbf_dynamic_init();

    /* eRPC client side initialization */
    erpc_client_init(transport, message_buffer_factory);

    /* other code like init matrix1 and matrix2 values */
    ...

    /* call eRPC functions */
    erpcMatrixMultiply(matrix1, matrix2, result_matrix);

    /* other code like print result matrix */
    ...

    return 0;
}
```

erpc_transport_cmsis_uart_init - usb version is in erpc_setup_usb_crc

Instead, make main init the usb, pass only handle in - hmmm, how do I config callbacks

Once everyting is initialized