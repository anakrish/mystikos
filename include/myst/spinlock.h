// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#ifndef _MYST_SPINLOCK_H
#define _MYST_SPINLOCK_H

#include <errno.h>
#include <myst/defs.h>
#include <myst/types.h>

typedef struct myst_spinlock
{
    volatile uint16_t ticket;
    volatile uint16_t serving;
} myst_spinlock_t;

#define MYST_SPINLOCK_INITIALIZER \
    {                             \
        0, 0                      \
    }

MYST_INLINE void myst_spinlock_init(myst_spinlock_t* s)
{
    s->ticket = 0;
    s->serving = 0;
}

MYST_INLINE void myst_spin_lock(myst_spinlock_t* s)
{
    int my_ticket = __atomic_fetch_add(&s->ticket, 1, __ATOMIC_ACQ_REL);
    while (__atomic_load_n(&s->serving, __ATOMIC_ACQUIRE) != my_ticket)
        asm volatile("pause" ::: "memory");
}

MYST_INLINE void myst_spin_unlock(myst_spinlock_t* s)
{
    __atomic_fetch_add(&s->serving, 1, __ATOMIC_RELEASE);
}

#endif /* _MYST_SPINLOCK_H */
