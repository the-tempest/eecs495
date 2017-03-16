#include <nautilus/nautilus.h>
#include <nautilus/window-manager.h>
#include <nautilus/thread.h>
#include <nautilus/ugui.h>
#include <nautilus/gui.h>
#include <nautilus/list.h>
#include <nautilus/vc.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


static struct list_head wm_window_list;
nk_thread_id_t wm_thread;
static struct nk_virtual_console *wm_cons;


// window manager's info on an app
typedef struct wm_app_s{
        nk_thread_id_t app_thread;
        struct list_head wm_node; 
} wm_app;

wm_app *cur_app;


UG_WINDOW *wm_get_app_window(wm_app *app)
{
        return vc_get_window(get_thread_vc(app->app_thread));
}

void wm_startup()
{
        wm_cons = nk_create_vc("Window Manager",
                               COOKED,
                               0,
                               NULL,
                               NULL);
        nk_bind_vc(get_cur_thread(), wm_cons);
}

void return_to_app(nk_thread_id_t app_thread)
{
        nk_sched_awaken(app_thread, 0);
        nk_sched_sleep();
}

void return_to_wm(nk_thread_id_t app_tid)
{
        INFO("In return to wm");
        struct list_head *cur;
        list_for_each(cur, &wm_window_list){
                wm_app *tmp_app =  list_entry(cur, wm_app, wm_node);
                INFO("Finding App");

                if(tmp_app->app_thread == app_tid){
                        cur_app = tmp_app;
                        INFO("Found it!");
                        break;
                }
        }
        INFO("waking wm_thread\n");
        nk_sched_awaken(get_wm_thread(), 0);
        INFO("going to sleep\n");
        nk_sched_sleep();
}



void wm_add_app(struct nk_thread *app_thread)
{
        wm_app *new_wm_app = malloc(sizeof(wm_app));
        new_wm_app->app_thread = app_thread;

        list_add(&new_wm_app->wm_node, &wm_window_list);
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

void wm_go(void* input, void ** output)
{
	INFO("in wm go\n");
        wm_startup();
	nk_sched_sleep();

	INFO("Window Manager Awakened\n");
        while(1){
                nk_keycode_t key = nk_dequeue_keycode(nk_get_cur_vc());

                if(key == NO_KEY) continue;

                switch(key){
                        /* resize: */
                case('w'):
                        break;
                case('s'):
                        break;
                case('a'):
                        break;
                case('d'):
                        break;
                        /* Move */
                case('i'):
                        break;
                case('k'):
                        break;
                case('j'):
                        break;
                case('l'):
                        break;
                        /* App Cycling */
                case('n'):
			INFO("Pressed next window\n");
                        cur_app = list_next_entry(&cur_app->wm_node,
                                                  wm_app,
                                                  wm_node);
			INFO("Switching to %x", cur_app->app_thread);
                        break;
                case('p'):
                        cur_app = list_prev_entry(&cur_app->wm_node,
                                                  wm_app,
                                                  wm_node);
                        break;
                        /* go to selected app */
                case('\r'):
                        return_to_app(cur_app->app_thread);
                        break;
                default:
                        break;
                }
                UG_WindowShow(wm_get_app_window(cur_app));
                gui_update();
        }
}

void wm_init()
{
        if(nk_thread_start(wm_go, // don't run, app will call us
                            NULL,
                            NULL,
                            false,
                            0,
                            &wm_thread,
                            0)){
                ERROR("Couldn't start wm thread");
        }
	
        INIT_LIST_HEAD(&wm_window_list);
}


nk_thread_id_t get_wm_thread()
{
        return wm_thread;
}
