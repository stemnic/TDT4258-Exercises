#include <types.h>

#define _r volatile const
#define _w volatile
#define _rw volatile

struct gpio_reg
{
    _rw u32 CTRL;
    _rw u32 MODEL;
    _rw u32 MODEH;
    _rw u32 DOUT;
    _w u32 DOUTSET;
    _w u32 DOUTCLR;
    _w u32 DOUTTGL;
    _r u32 DIN;
    _rw u32 LOCK;
};

#define GPIOA ((struct gpio_reg *)0x40006000)
#define GPIOB ((struct gpio_reg *)0x40006024)
#define GPIOC ((struct gpio_reg *)0x40006048)

struct gpio_irq
{
    _rw u32 EXTIPSELL;
    _rw u32 EXTIPSELH;
    _rw u32 EXTRISE;
    _rw u32 EXTIFALL;
    _rw u32 IEN;
    _r u32 IF;
    _w u32 IFS;
    _w u32 IFC;
};

#define GPIO_IRQ ((struct gpio_reg *)0x40006100)

typedef struct
{
    _rw u32 ISER[8];
    _r u32 RESERVED1[24];
    _rw u32 ICER[8];
    _r u32 RESERVED2[24];
    _rw u32 ISPR[8];
    _r u32 RESERVED3[24];
    _rw u32 ICPR[8];
    _r u32 RESERVED4[24];
    _rw u32 IABR[8];
    _r u32 RESERVED5[56];
    _rw u8 IPR[240];
    _r u32 RESERVED6[644];
    _r u32 STIR;
} nvic_reg;

#define NVIC ((nvic_reg *)0xE000E100)
