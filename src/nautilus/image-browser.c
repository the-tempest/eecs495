// A simple image browser.


// Keyboard presses go to next image and previous image.

// should create a ugui window on startup that is the size of the whole screen,
// and move itself to the foreground. At which point the application loop should
// take over.


// I'd recommend either a linked list or an array of images, doesn't really matter.

// Look at the old commits on the image branch for how to show images and include them 
#include <nautilus/image-browser.h>

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
#include <image/eye.h>
#include <image/green.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


void image_logic(UG_MESSAGE * msg){
        return;
}

void image_browser() {
	
	UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));
	UG_OBJECT* objlst2 = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));	
	struct nk_virtual_console *new_vc = nk_create_vc ("Image Browser" , COOKED , 0 , NULL , NULL );

        if (!new_vc) {
                ERROR("Failed to allocate new console\n");
                return;
        }
	    if (nk_bind_vc(get_cur_thread(), new_vc)) {
	                ERROR("couldn't bind vc to thread");
	                return;
	        }

	vc_set_window(new_vc, (UG_WINDOW *) malloc(sizeof(UG_WINDOW)));
	UG_WINDOW * image_window = (UG_WINDOW *) malloc(1 * sizeof(UG_WINDOW));
	
	if(UG_WindowCreate(image_window, objlst, max_objs, image_logic)){
                ERROR("Couldn't create window");
        }
        UG_IMAGE eye_img, green_img;
    	const UG_U32 eye_bits[] = eye_array;
        const UG_BMP eye_bmp = {
                 (void *) eye_bits,
                 500,
                 500,
                 BMP_BPP_32,
                 BMP_RGB888
         };
      const UG_U32 green_bits[] = green_array;
      const UG_BMP green_bmp = {
                 (void *) green_bits,
                 50,
                 50,
                 BMP_BPP_32,
                 BMP_RGB888
         };   



    const UG_32 image_array[] = {eye_array , green_array};
    UG_IMAGE img_array[] = {eye_img , green_img};
	UG_WindowSetTitleText(image_window, "Image Browser");
    UG_WindowSetTitleTextFont(image_window, &FONT_12X16);
    UG_WindowSetTitleTextColor(image_window, C_RED);
	UG_WindowSetTitleInactiveColor(image_window,C_GRAY ) ;
	

	
	int image_count = 0;
	  if (UG_WindowShow(image_window)) {
                ERROR("Window creation failure");
        }
        gui_update();
       int img_array_count = 0;
    
	while(1){
	    nk_keycode_t key = nk_dequeue_keycode(new_vc);
	    if(key != NO_KEY && image_count < 2 && image_count != -1) {
		
		switch (key) {
			////case F1:
				//TODO: call return window manager
			case 'j':
				//TODO:call display_image
				next_image(image_window, key , image_count);
				break;
			case 'k':
				//TODO:call display image
			prev_image(image_window, key , image_count);
				break;
			case KEY_LCTRL :
				//shutdown_app(get_cur_thread);
				//nk_destroy_vc(new_vc);
				exit(0);

			
		}
		gui_update();
	    }
  	}
  	//this function displays previous and next images . #TODO - Edge case not handled!
  	void next_image(UG_WINDOW image_window,nk_keycode_t key , int image_count){
  		UG_ImageCreate( &image_window , &img_array[img_array_count] , IMG_ID image_count , 0 , 0 ,1024, 768) ;
		UG_ImageSetBMP (&image_window , IMG_ID image_count , &image_array[image_count] ) ;
		UG_WindowShow(&image_window) ;
		UG_ImageShow ( &image_window , IMG_ID image_count) ;
		image_count++;
		img_array_count++;
  		return;
  	}
  	void prev_image(UG_WINDOW image_window,nk_keycode_t key , int image_count){
  		image_count--;
  		img_array_count--;
  		UG_ImageCreate( &image_window , &img_array[img_array_count] , IMG_ID image_count , 0 , 0 , 1024 , 768 ) ;
		UG_ImageSetBMP (&image_window , IMG_ID image_count , &image_array[image_count] ) ;
		UG_ImageShow ( &image_window , IMG_ID image_count) ;
  		return;
  	}

}
