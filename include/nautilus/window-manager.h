#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <nautilus/thread.h>

void return_to_wm(nk_thread_id_t app_tid);

void wm_add_app(struct nk_thread *app_thread);

void wm_startup();

void wm_init();

nk_thread_id_t get_wm_thread();

#endif /* WINDOW-MANAGER_H */
