#include <nautilus/gui.h>
#include <nautilus/nautilus.h>
#include <nautilus/desktop.h>
#include <nautilus/intrinsics.h>
#include <dev/kbd.h>
#include <nautilus/ugui.h>
#include <dev/vesa.h>
#include <nautilus/thread.h>
#include <nautilus/vc.h>
#include <nautilus/atomic.h>
#include <nautilus/timer.h>

#include <nautilus/image-browser.h>
#include <nautilus/window-manager.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


// The desktop.

// needs buttons that display labels and call callbacks.

// The active button should be highlighted somehow.

// When you press enter, it calls that button's callback, which can launch an app,
// or do other things.

nk_thread_id_t desktop_thread;

nk_thread_id_t get_desktop_thread()
{
        return desktop_thread;
}

void desktop_logic(UG_MESSAGE * msg){
        return;
}

void (*app_launchers[])(void) = {cats_startup,
                               memes_startup,
                               slides_startup};

void desktop_init() {

        desktop_thread = get_cur_thread();

        UG_BUTTON * button_1 = (UG_BUTTON *) malloc(sizeof(UG_BUTTON));
        UG_BUTTON * button_2 = (UG_BUTTON *) malloc(sizeof(UG_BUTTON));
        UG_BUTTON * button_3 = (UG_BUTTON *) malloc(sizeof(UG_BUTTON));
        struct nk_virtual_console *cons = nk_get_cur_vc();
        if (nk_bind_vc(get_cur_thread(), cons)) {
                ERROR("couldn't bind vc to thread");
                return;
        }


        vc_set_window(cons, (UG_WINDOW *) malloc(sizeof(UG_WINDOW)));
        UG_WINDOW * desktop_window = vc_get_window(cons);


        UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));
        UG_OBJECT* objlst2 = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));

        if(UG_WindowCreate(desktop_window, objlst, max_objs, desktop_logic)){
                ERROR("Couldn't create window");
        }

        UG_WindowSetTitleText(desktop_window, "Desktop");
        UG_WindowSetTitleTextFont(desktop_window, &FONT_12X16);
        UG_WindowSetTitleTextColor(desktop_window, C_GREEN);
        UG_WindowSetTitleInactiveColor(desktop_window,C_GRAY ) ;
        UG_ButtonCreate(desktop_window, button_1, BTN_ID_0, 30, 30, 180, 80);
        UG_ButtonSetBackColor(desktop_window, BTN_ID_0, C_YELLOW);
        UG_ButtonSetAlternateBackColor(desktop_window, BTN_ID_0, C_BLUE);
        UG_ButtonSetStyle (desktop_window , BTN_ID_0 , BTN_STYLE_3D );
        UG_ButtonShow(desktop_window, BTN_ID_0);
        UG_ButtonSetText(desktop_window, BTN_ID_0, "Cats");

        UG_ButtonCreate(desktop_window, button_2, BTN_ID_1, 200, 30, 360, 80);
        UG_ButtonShow(desktop_window, BTN_ID_1);
        UG_ButtonSetBackColor(desktop_window, BTN_ID_1, C_BLUE);
        UG_ButtonSetAlternateBackColor(desktop_window, BTN_ID_1, C_YELLOW);
        UG_ButtonSetStyle (desktop_window , BTN_ID_1 , BTN_STYLE_3D );
        UG_ButtonSetText(desktop_window, BTN_ID_1, "Memes");

        UG_ButtonCreate(desktop_window, button_3, BTN_ID_2, 380, 30, 540, 80);
        UG_ButtonShow(desktop_window, BTN_ID_2);
        UG_ButtonSetBackColor(desktop_window, BTN_ID_2, C_BLUE);
        UG_ButtonSetAlternateBackColor(desktop_window, BTN_ID_2, C_YELLOW);
        UG_ButtonSetStyle (desktop_window , BTN_ID_2, BTN_STYLE_3D );
        UG_ButtonSetText(desktop_window, BTN_ID_2, "Slides");

        if (UG_WindowShow(desktop_window)) {
                ERROR("Window creation failure");
        }
        gui_update();
        int curr_icon = 0;
        UG_WINDOW * windows = (UG_WINDOW *) malloc(3 * sizeof(UG_WINDOW));
        UG_WindowCreate(&windows[2], objlst2, max_objs, desktop_logic);
        UG_WindowResize(&windows[2], 400, 400, 600, 600);
        //UG_WindowShow(&windows[2]);

        //wm_init();

        //wm_add_app(get_cur_thread());

        while(1){
                nk_keycode_t key = nk_dequeue_keycode(cons);
                if(key != NO_KEY) {

                        switch (key) {
                        case KEY_KPRIGHT:
                                UG_ButtonSetBackColor(desktop_window, curr_icon, C_BLUE);
                                curr_icon = (curr_icon+1)%3;
                                UG_ButtonSetBackColor(desktop_window, curr_icon, C_YELLOW);
                                break;
                        case KEY_KPLEFT:
                                UG_ButtonSetBackColor(desktop_window, curr_icon, C_BLUE);
                                curr_icon = curr_icon <= 0 ? 2 : curr_icon - 1;
                                UG_ButtonSetBackColor(desktop_window, curr_icon, C_YELLOW);
                                break;
                        case '\r':
                                printk("enter");
                                app_launchers[curr_icon]();
				printk("The deskto thread: %d", get_cur_thread());
                                nk_sched_sleep();
                                break;
                        }
			UG_WindowShow(desktop_window);
                        gui_update();
                }
        }
}
