#ifndef DESKTOP_H
#define DESKTOP_H

#include <nautilus/thread.h>

void desktop_init();

nk_thread_id_t get_desktop_thread();

#endif /* DESKTOP_H */

