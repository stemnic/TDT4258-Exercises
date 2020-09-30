#ifndef NVIC_H
#define NVIC_H

#include <types.h>
#include <hardware.h>
#include <cpu.h>
/*
 * The processor only implements 3 priority bits
 */
enum irq_priority
{
    NVIC_PRI_0,
    NVIC_PRI_1,
    NVIC_PRI_2,
    NVIC_PRI_3,
    NVIC_PRI_4,
    NVIC_PRI_5,
    NVIC_PRI_6,
    NVIC_PRI_7
};

/*
 * Enable an NVIC interrupt vector
 */
static inline void nvic_enable(u8 irq_number)
{
    NVIC->ISER[irq_number >> 5] = (1 << (irq_number & 0b11111));
}

/*
 * Disable an NVIC interrupt vector
 */
static inline void nvic_disable(u8 irq_number)
{
    NVIC->ICER[irq_number >> 5] = (1 << (irq_number & 0b11111));
    dsb();
    isb();
}

/*
 * Checks whether an NVIC interrupt vector is enabled
 */
static inline u8 nvic_is_enabled(u8 irq_number)
{
    if (NVIC->ISER[irq_number >> 5] & (1 << (irq_number & 0b11111)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Pends the execution of an NVIC interrupt vector
 */
static inline void nvic_set_pending(u8 irq_number)
{
    NVIC->ISPR[irq_number >> 5] = (1 << (irq_number & 0b11111));
}

/*
 * Clear the pending flag of an NVIC interrupt vector
 */
static inline void nvic_clear_pending(u8 irq_number)
{
    NVIC->ICPR[irq_number >> 5] = (1 << (irq_number & 0b11111));
}

/*
 * Checks whether an NVIC interrupt vector is pending
 */
static inline u8 nvic_is_pending(u8 irq_number)
{
    if (NVIC->ISPR[irq_number >> 5] & (1 << (irq_number & 0b11111)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Checks whether an NVIC interrupt vector is active or active and pending
 */
static inline u8 nvic_is_active(u8 irq_number)
{
    if (NVIC->IABR[irq_number >> 5] & (1 << (irq_number & 0b11111))) {
        return 1;
    } else {
        return 0;
    }
}

/* 
 * Sets the priority of an NVIC interrupt vector
 */
static inline void nvic_set_priority(u8 irq_number, enum irq_priority pri)
{
    NVIC->IPR[irq_number] = (u8)((pri << 5) & 0xFF);
}

/*
 * Gets the priority of an NVIC interrupt vector
 */
static inline u8 nvic_get_priority(u8 irq_number)
{
    return NVIC->IPR[irq_number] >> 5;
}

#endif
