#include <nautilus/gui.h>
#include <nautilus/nautilus.h>
#include <nautilus/intrinsics.h>
#include <nautilus/ugui.h>
#include <dev/vesa.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


UG_GUI the_gui;

void gui_init(UG_GUI *the_gui){
        struct vesa_mode_request r;
        vesa_mode_t mode;

        r.width=1024;
        r.height=768;
        r.bpp=32;
        r.text=0;
        r.lfb=1;

        INFO("Looking for a 1024x768x32 graphics mode\n");
        if(vesa_find_matching_mode(&r,&mode)) {
                INFO("Cannot find a matching mode\n");
        }

        if(vesa_set_cur_mode(mode)) {
                ERROR("Couldn't set mode\n");
        }

        UG_Init(the_gui, ugui_draw_pixel, r.width, r.height);

        UG_FontSelect(&FONT_12X16);
        UG_ConsoleSetBackcolor(C_BLACK);
        UG_ConsoleSetForecolor(C_WHITE);

        UG_ConsolePutString("Initializing GUI\n");
}

void desktop_logic(UG_MESSAGE * msg){
        return;
}

void desktop_init(){
        struct nk_virtual_console *cons = get_cur_vc();
        if (nk_bind_vc(get_cur_thread(), cons)) {
                ERROR("couldn't bind vc to thread");
                return -1;
        }

        cons->window = (UG_WINDOW *) malloc(sizeof(UG_WINDOW));

        const UG_U8 max_objs = 10;

        UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));

        if(UG_WindowCreate(cons->window, objlst, max_objs, desktop_logic)){
                ERROR("Couldn't create window");
        }

        UG_WindowSetTitleText(&cons->window, "Fuck everything");
        UG_WindowShow(&cons->window);
}
