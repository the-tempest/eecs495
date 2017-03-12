#include <nautilus/gui.h>
#include <nautilus/nautilus.h>
#include <nautilus/intrinsics.h>
#include <dev/kbd.h>
#include <nautilus/ugui.h>
#include <dev/vesa.h>
#include <nautilus/thread.h>
#include <nautilus/vc.h>
#include <nautilus/atomic.h>
#include <image/dandelion.h>
#include <image/eye.h>
#include <image/green.h>
#include <nautilus/timer.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)

void desktop_init();
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

void desktop_logic(UG_MESSAGE * msg){
  return;
}

void desktop_init() {

        struct nk_virtual_console *cons = nk_get_cur_vc();
        if (nk_bind_vc(get_cur_thread(), cons)) {
                ERROR("couldn't bind vc to thread");
                return;
        }


        UG_TEXTBOX * textbox_1 = (UG_TEXTBOX *) malloc(sizeof(UG_TEXTBOX));
        vc_set_window(cons, (UG_WINDOW *) malloc(sizeof(UG_WINDOW)));
        UG_WINDOW * desktop_window = vc_get_window(cons);
	UG_WINDOW secondary_window;
	UG_BUTTON button_1;
        // DINDA
        UG_IMAGE eye_img;
	UG_IMAGE eye_img_2;
        const UG_U32 d_bmp [] ={};
        const UG_BMP d = {
                (void*) d_bmp,
                590,
                393,
                BMP_BPP_32,
                BMP_RGB888
        };
        const UG_U8 max_objs = 10;
        UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));

        UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));
	UG_OBJECT* objlst2 = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));
	
        if(UG_WindowCreate(desktop_window, objlst, max_objs, desktop_logic)){
                ERROR("Couldn't create window");
        }
	UG_WindowCreate(&secondary_window, objlst2, max_objs, desktop_logic);
	UG_ButtonCreate(&secondary_window, &button_1, BTN_ID_0, 10, 10, 110, 60);
	//UG_ButtonShow(&secondary_window, BTN_ID_0);
	UG_WindowSetBackColor(&secondary_window, C_GREEN);
	UG_WindowShow(&secondary_window);        

	//if (UG_WindowSetForeColor(desktop_window , C_WHITE)) {
        //	ERROR("Couldn't set fore color :(");
        //}
        //if (UG_WindowSetBackColor (desktop_window , C_GREEN)) {
        //	ERROR("Couldn't set back color :(");
        //}
        const UG_U32 eye_bits[] = eye_array;
        const UG_BMP eye_bmp = {
                (void *) eye_bits,
                640,
                624,
                BMP_BPP_32,
                BMP_RGB888
        };
	const UG_BMP eye_bmp2 = {
		(void *) eye_bits,
		300,
		200,
		BMP_BPP_32,
		BMP_RGB888
	};
        //UG_ImageCreate(desktop_window, &eye_img, IMG_ID_0, 0, 0, 1024, 768);
        //UG_ImageSetBMP(desktop_window, IMG_ID_0, &eye_bmp);
	UG_ImageCreate(desktop_window, &eye_img_2, IMG_ID_1, 0, 0, 300, 200);
	UG_ImageSetBMP(desktop_window, IMG_ID_1, &eye_bmp);
        UG_WindowSetTitleText(desktop_window, "F");
        UG_WindowSetTitleTextFont(desktop_window, &FONT_12X16);
        UG_WindowSetTitleTextColor(desktop_window, C_GREEN);
        
	 
	//UG_TextboxCreate(desktop_window, textbox_1, TXB_ID_0, 120, 10, 640, 200);
        //UG_TextboxSetText(desktop_window, TXB_ID_0, "F U C K  E V E R Y T H I N G ");
        //UG_TextboxSetBackColor(desktop_window, TXB_ID_0, C_BLACK);
        if (UG_WindowShow(desktop_window)) {
                ERROR("Window creation failure");
        }
        /*
          char buf[MAX_CMD];

          while (1) {
          nk_vc_gets(buf,MAX_CMD,1);
          UG_TextboxSetText(desktop_window, TXB_ID_0, buf);
          UG_Update();
          }
        */
        UG_Update();	
        nk_delay(1);
	UG_ImageDelete(desktop_window, IMG_ID_1);
	UG_WindowResize(desktop_window, 200, 150, 300, 200);
	UG_WindowResize(&secondary_window, 400, 400, 600, 600);
	UG_WindowSetBackColor(desktop_window, C_RED);
	UG_WindowShow(&secondary_window);
        UG_Update();	
        nk_delay(1);
	UG_WindowResize(&secondary_window, 0,0,500, 600);
	UG_WindowShow(&secondary_window);
	UG_Update();
	nk_delay(1);
	UG_WindowShow(desktop_window);	
	gui_update();
        /*
          char buf[MAX_CMD];

          while (1) {
          nk_vc_gets(buf,MAX_CMD,1);
          UG_TextboxSetText(desktop_window, TXB_ID_0, buf);
          UG_Update();
          }
        */
}
