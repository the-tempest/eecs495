#include <cga.h>
#include <printk.h>
#include <types.h>
#include <paging.h>
#include <idt.h>
#include <spinlock.h>

void 
main (unsigned long mbd, unsigned long magic)
{
    int *x;
    term_init();

    printk("Welcome to the Nautilus Kernel\n\n");

    init_page_frame_alloc(mbd);

    setup_idt();

    // test the new idt with a bad pointer...
    x = (int*)0xffffe000;
    *x = 10;

    spinlock_t lock;
    spinlock_init(&lock);
    spin_lock(&lock);
    *x = 30;
    spin_unlock(&lock);

}
