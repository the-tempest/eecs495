#include <nautilus/gui.h>

#include <nautilus/nautilus.h>
#include <nautilus/intrinsics.h>
#include <dev/kbd.h>
#include <nautilus/ugui.h>
#include <dev/vesa.h>
#include <nautilus/thread.h>
#include <nautilus/vc.h>
#include <nautilus/atomic.h>
#include <nautilus/timer.h>

#include <nautilus/desktop.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)

//void desktop_init();
uint8_t gui_dirty;
nk_thread_id_t tid;
UG_GUI the_gui;

////////////////////////////////////////////////////////////////////////////////
// Gui thread:
void gui_update_worker(void* input, void** output){

        while(1){
                if(gui_dirty){
                        while(gui_dirty){
                                atomic_and(gui_dirty, 0);
                                UG_Update();
                        }
                }
                nk_yield();
        }
}

void gui_thread_launch(){
        nk_thread_start(gui_update_worker,
                        NULL,
                        NULL,
                        true,
                        false,
                        &tid,
                        CPU_ANY);
}

void gui_update(){
        atomic_or(gui_dirty, 1);
}

////////////////////////////////////////////////////////////////////////////////
// Init:
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
        gui_thread_launch();
      	desktop_init();
}

