#include <nautilus/nautilus.h>
#include <nautilus/window-manager.h>
#include <nautilus/thread.h>
#include <nautilus/ugui.h>
#include <nautilus/list.h>
#include <nautilus/vc.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


static struct list_head window_list;

static nk_thread_id_t wm_thread;

//TODO initialize

typedef struct wm_vc_s{
        struct nk_virtual_console *cons;
        nk_thread_id_t thread;
        struct list_head node; // need another node to preserve global list
} wm_app;


nk_thread_fun_t wm_go(void* input, void ** output)
{
        while(1){
                
        }
}

void wm_init()
{
        if(nk_thread_create(wm_go, // don't run, app will call us
                            NULL,
                            NULL,
                            false,
                            0,
                            wm_thread,
                            CPU_ANY)){
                ERROR("Couldn't start wm thread");
        }
        INIT_LIST_HEAD(&window_list);
}

void return_to_wm()
{
        nk_sched_awaken(&wm_thread, CPU_ANY);
        nk_sched_sleep();
}

void return_to_app(struct nk_thread *app_thread)
{
        nk_sched_awaken(app_thread, CPU_ANY);
}

wm_app *wm_add_window(struct nk_virtual_console *new_app_cons, struct nk_thread *app_thread)
{
        wm_app *new_wm_app = malloc(sizeof(wm_app));
        new_wm_app->cons = new_app_cons;
        new_wm_app->thread = app_thread;

        list_add(&new_wm_app->node, &window_list);

        return new_wm_app;
}

void wm_remove_window(wm_app* victim_window)
{
        //TODO
}

void shutdown_app(nk_thread_id_t app)
{
        //TODO
}

void wm_shutdown()
{
        // TODO: needs to free list of windows
}
