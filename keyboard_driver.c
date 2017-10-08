#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");

/*
 This function services keyboard interrupts.
*/
irq_handler_t irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
	static unsigned char scancode;
	
	/*
	 Read keyboard status
	*/
	scancode = inb (0x60);

	if ((scancode == 0x01) || (scancode == 0x81))
	{
		printk ("\nYou pressed Esc !");
	}

	return (irq_handler_t) IRQ_HANDLED;
}

/*
 Initialize the module − register the IRQ handler
*/
int init_module(void)
{
	/*
 	 Request IRQ 1, the keyboard IRQ, to go to our irq_handler SA_SHIRQ means we're willing to have othe handlers on this IRQ. SA_INTERRUPT can be used to make the handler into a fast interrupt.
	*/
	return request_irq (1, (irq_handler_t) irq_handler, IRQF_SHARED, "test_keyboard_irq_handler", (void *)(irq_handler));
}

void cleanup_module(void) {
	
}


