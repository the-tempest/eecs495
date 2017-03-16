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

// images
#include <image/naut_splash.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)

//void desktop_init();
uint8_t gui_dirty;
nk_thread_id_t tid;
UG_GUI the_gui;
static spinlock_t gui_lock;


////////////////////////////////////////////////////////////////////////////////
// Gui thread:

void gui_update_worker(void* input, void** output){

        while(1){
                if(gui_dirty){
                        while(gui_dirty){
                                atomic_and(gui_dirty, 0);
                                uint8_t flags = spin_lock_irq_save(&gui_lock);
                                UG_Update();
                                spin_unlock_irq_restore(&gui_lock, flags);
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

const int border_size = 50;
////////////////////////////////////////////////////////////////////////////////
// Init:
void gui_init(UG_GUI *the_gui){
        struct vesa_mode_request r;
        vesa_mode_t mode;

        spinlock_init(&gui_lock);

        r.width=1280;
        r.height=1024;
        r.bpp=32;
        r.text=0;
        r.lfb=1;

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


        UG_WINDOW splash_window;
        UG_OBJECT objlst[10];
        UG_WindowCreate(&splash_window, objlst, 10, NULL);

        UG_IMAGE im_1;
        const UG_U32 splash_bits[] = naut_splash_array;
        const UG_BMP splash = {
                (void*) splash_bits,
                naut_splashwidth, naut_splashheight,
                BMP_BPP_32,
                BMP_RGB888
        };

        UG_ImageCreate(&splash_window, &im_1,
                       IMG_ID_0,
                       border_size,border_size,
                       naut_splashwidth + border_size,
                       naut_splashheight + border_size);
        UG_ImageSetBMP(&splash_window, IMG_ID_0, &splash);

        UG_WindowShow(&splash_window);
        UG_Update();

        //nk_delay(1);

        UG_ConsolePutString("Initializing GUI\n");

        gui_thread_launch();
      	desktop_init();
}
