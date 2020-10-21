/*
 * This is a gamepad Linux kernel module.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioport.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/kdev_t.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/signal.h>
#include <asm/siginfo.h>
#include <linux/interrupt.h>
#include "efm32gg.h"

// Defines
#define GPIO_EVEN_IRQ_LINE 17
#define GPIO_ODD_IRQ_LINE 18
#define DRIVER_NAME "gamepad"
#define DEV_NR_COUNT 1

// Prototypes
static int __init gamepad_init(void);
static void __exit gamepad_exit(void);
static int gamepad_open(struct inode *, struct file *);
static int gamepad_release(struct inode *, struct file *);
static ssize_t gamepad_read(struct file *, char *__user, size_t, loff_t *);
static ssize_t gamepad_write(struct file *, char *__user, size_t, loff_t *);
static irqreturn_t gpio_interrupt_handler(int, void *, struct pt_regs *);
static int gamepad_fasync(int, struct file *, int mode);

static int memory_allocate(void);
static inline int memory_free(void);
static inline int gpio_enable(void);
static inline int gpio_disable(void);

// Static variables
static dev_t device_nr;
struct cdev gamepad_cdev;
struct fasync_struct *async_queue;
struct class *cl;
static void __iomem *mem_gpio_port_c, *mem_gpio_int;

// Module configuration

MODULE_DESCRIPTION("Gamepad module, very useful.");
MODULE_LICENSE("GPL");
module_init(gamepad_init);
module_exit(gamepad_cleanup);

static struct file_operations gamepad_fops = {
	.owner = THIS_MODULE,
	.open = gamepad_open,
	.release = gamepad_release,
	.read = gamepad_read,
	.write = gamepad_write,
	.fasync = gamepad_fasync,
};

irqreturn_t gpio_interrupt_handler(int irq, void* dev_id, struct pt_regs* regs)
{
    printk(KERN_ALERT "Handling GPIO interrupt\n");
    iowrite32(ioread32(GPIO_IF), GPIO_IFC);
    if (async_queue) {
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}

static int gamepad_fasync(int fd, struct file* filp, int mode) 
{
    return fasync_helper(fd, filp, mode, &async_queue);
}

/*
 * gamepad_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init gamepad_init(void)
{
	printk("Hello World, gamepad module here\n");

	if (alloc_chrdev_region(&device_nr, 0, DEV_NR_COUNT, DRIVER_NAME) < 0)
	{
		printk(KERN_ALERT "Failed to allocate device numbers\n");
		return -1;
	}

	if (memory_allocate() < 0)
	{
		printk(KERN_ALERT "Failed to access GPIO memory region");
		memory_free();
		return -1;
	}

	// Configure buttons as inputs
	// and enable interrupts:
	gpio_enable();

	// Bind our IRQs in the kernel:
	request_irq(GPIO_EVEN_IRQ_LINE, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);
    request_irq(GPIO_ODD_IRQ_LINE, (irq_handler_t)gpio_interrupt_handler, 0, DRIVER_NAME, &gamepad_cdev);

	return 0;
}

/*
 * gamepad_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit gamepad_cleanup(void)
{
	printk("Unloading gamepad driver from kernel...\n");
	
	gpio_disable();
	
	free_irq(GPIO_EVEN_IRQ_LINE, &gamepad_cdev);
    free_irq(GPIO_ODD_IRQ_LINE, &gamepad_cdev);

	iounmap(mem_gpio_port_c);
	iounmap(mem_gpio_int);
	release_mem_region(GPIO_PA_BASE, 0x01C);
	release_mem_region(GPIO_PC_BASE, 0x020);
	release_mem_region(GPIO_INT_BASE, 0x020);

	class_destroy(cl);
	cdev_del(&my_cdev);

	unregister_chrdev_region(DEV_ID, 1);
}


static int gamepad_open(struct inode *, struct file *)
{
	printk(KERN_INFO "Gamepad driver has started\n");
	return 0;
}


static int gamepad_release(struct inode *, struct file *)
{
	printk(KERN_INFO "Gamepad driver has been closed\n");
	return 0;
}


static ssize_t gamepad_read(struct file* filp, char* __user buff, size_t count, loff_t* offp)
{
    /*
	 * Read gpio button status and write to buff
	 * This isn't used by the program, because button handling is done using fasync 
	 */
    uint32_t data = ioread32(GPIO_PC_DIN);
    copy_to_user(buff, &data, 1);
    return 1;
}

static ssize_t gamepad_write(struct file* filp, char* __user buff, size_t count, loff_t* offp)
{
	/* This doesn't do anything because gamepad only uses buttons, so nothing to write to */
    return 1;
}

/*
 * HELPER FUNCTIONS: 
 */

static int memory_allocate(void)
{
	// Request access to the required memory regions
	if (request_mem_region(GPIO_PA_BASE, 0x01C, DEV_NAME) == NULL)
		return -1;
	if (request_mem_region(GPIO_PC_BASE, 0x020, DEV_NAME) == NULL)
		return -1;
	if (request_mem_region(GPIO_INT_BASE, 0x020, DEV_NAME) == NULL)
		return -1;

	mem_gpio_port_c = ioremap_nocache(GPIO_PC_BASE, 0x020);
	if (mem_gpio_port_c == 0) {
		printk(KERN_ERR "Failed to remap GPIO port C\n");
		return -1;
	}

	mem_gpio_int = ioremap_nocache(GPIO_INT_BASE, 0x020);
	if (mem_gpio_int == 0) {
		printk(KERN_ERR "Failed to remap GPIO interrupts\n");
		return -1;
	}

	return 0;
}

static inline int memory_free(void)
{
	iounmap(mem_gpio_port_c);
	iounmap(mem_gpio_int);
	release_mem_region(GPIO_PA_BASE, 0x01C);
	release_mem_region(GPIO_PC_BASE, 0x020);
	release_mem_region(GPIO_INT_BASE, 0x020);

	return 0;
}

static inline int gpio_enable(void)
{
	// Set pins C0-C7 as input with filter:
	iowrite32(0x33333333, mem_gpio_port_c + MODEL_OFFSET);
	// Enable pull-up resistors:
	iowrite32(0xFF, mem_gpio_port_c + DOUT_OFFSET);

	// Set interrupts for pins C0-7:
	iowrite32(0x22222222, mem_gpio_int + EXTIPSELL_OFFSET);
	iowrite32(0xFF, mem_gpio_int + EXTIFALL_OFFSET);

	// Enable interrupts for pins C0-7:
	iowrite32(0xFF, mem_gpio_int + IEN_OFFSET);

	return 0;
}

static inline int gpio_disable(void)
{
	// Disable interrupts for pins C0-7:
	iowrite32(0x00, mem_gpio_int + IEN_OFFSET);
}