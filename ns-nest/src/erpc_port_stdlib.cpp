/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * Copyright 2021 ACRIOS Systems s.r.o.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_port.h"

#include <cstdlib>
#include <new>
#include "ns-malloc.h"
#include "ns_ambiqsuite_harness.h"

using namespace std;

void *operator new(size_t count) THROW_BADALLOC
{
    void *p = erpc_malloc(count);
    return p;
}

void *operator new(size_t count, const nothrow_t &tag) THROW NOEXCEPT
{
    (void)tag;
    void *p = erpc_malloc(count);
    return p;
}

void *operator new[](size_t count) THROW_BADALLOC
{
    void *p = erpc_malloc(count);
    return p;
}

void *operator new[](size_t count, const nothrow_t &tag) THROW NOEXCEPT
{
    (void)tag;
    void *p = erpc_malloc(count);
    return p;
}

void operator delete(void *ptr) THROW NOEXCEPT
{
    erpc_free(ptr);
}

void operator delete(void *ptr, std::size_t count) THROW NOEXCEPT
{
    (void)count;
    erpc_free(ptr);
}

void operator delete[](void *ptr) THROW NOEXCEPT
{
    erpc_free(ptr);
}

void operator delete[](void *ptr, std::size_t count) THROW NOEXCEPT
{
    (void)count;
    erpc_free(ptr);
}

void *erpc_malloc(size_t size)
{
    void *p = ns_malloc(size);
    // ns_printf("erpc malloc for %d, got 0x%x\n", size, p);
    return p;
}

void erpc_free(void *ptr)
{
    ns_free(ptr);
    // ns_printf("erpc free for 0x%x\n", ptr);

}

/* Provide function for pure virtual call to avoid huge demangling code being linked in ARM GCC */
#if ((defined(__GNUC__)) && (defined(__arm__)))
extern "C" void __cxa_pure_virtual(void)
{
    for (;;)
    {
    };
}
#endif
