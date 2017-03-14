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
#include <nautilus/window-manager.h>
#include <image/eye.h>
#include <image/green.h>

#include<nautilus/desktop.h>
#include<nautilus/window-manager.h>

#define ERROR(fmt, args...) ERROR_PRINT("gui: " fmt, ##args)
#define DEBUG(fmt, args...) DEBUG_PRINT("gui: " fmt, ##args)
#define INFO(fmt, args...) INFO_PRINT("gui: " fmt, ##args)


static UG_BMP * bmp_array[2]; //= {&eye_bmp , &green_bmp};
static UG_IMAGE * img_array[2]; // = {&eye_img , &green_img};

nk_thread_id_t image_browser_thread;

void image_logic(UG_MESSAGE * msg){
        return;
}

void next_image(UG_WINDOW * image_window, int image_count){
        if (image_count != 0) {
                UG_ImageHide(image_window, image_count-1);
        }
        UG_ImageCreate( image_window , img_array[image_count] , image_count , 0 , 0 ,1024, 768) ;
        UG_ImageSetBMP (image_window , image_count , bmp_array[image_count] ) ;
        UG_WindowShow(image_window) ;
        UG_ImageShow (image_window , image_count) ;
        return;
}
void prev_image(UG_WINDOW * image_window, int image_count){

        UG_ImageShow(image_window, image_count-1);

        return;
}



void image_browser_go(void* input, void** output)
{
        INFO("Running on thread %d\n", get_cur_thread());

        UG_OBJECT* objlst = (UG_OBJECT *) malloc(max_objs * sizeof(UG_OBJECT));
        struct nk_virtual_console *new_vc = nk_create_vc ("Image Browser" , COOKED , 0 , NULL , NULL );

        if (!new_vc) {
                ERROR("Failed to allocate new console\n");
                return;
        }
        if (nk_bind_vc(get_cur_thread(), new_vc)) {
                ERROR("couldn't bind vc to thread");
                return;
        }
        DEBUG("At line 81\n");
        vc_set_window(new_vc, (UG_WINDOW *) malloc(sizeof(UG_WINDOW)));
        UG_WINDOW * image_window = (UG_WINDOW *) malloc(1 * sizeof(UG_WINDOW));

        DEBUG("At line 85\n");
        if(UG_WindowCreate(image_window, objlst, max_objs, image_logic)){
                ERROR("Couldn't create window");
        }
        DEBUG("At line 89\n");
        UG_IMAGE eye_img, green_img;
        UG_U32 eye_bits[] = eye_array;
        const UG_BMP eye_bmp = {
                (void *) eye_bits,
                500,
                500,
                BMP_BPP_32,
                BMP_RGB888
        };
        UG_U32 green_bits[] = green_array;
        const UG_BMP green_bmp = {
                (void *) green_bits,
                50,
                50,
                BMP_BPP_32,
                BMP_RGB888
        };
        DEBUG("at line 105");

        //UG_ImageCreate(image_window, &eye_img, IMG_ID_0, 0, 0, 1024, 768);
        //UG_ImageCreate(image_window, &green_img, IMG_ID_1, 0, 0, 1024, 768);
        //UG_ImageSetBMP(image_window, IMG_ID_0, &eye_bmp);
        //UG_ImageSetBMP(image_window, IMG_ID_1, &green_bmp);
        //UG_ImageShow(image_window, IMG_ID_0);
        //UG_ImageShow(image_window, IMG_ID_1);

        UG_WindowSetTitleText(image_window, "Image Browser");
        UG_WindowSetTitleTextFont(image_window, &FONT_12X16);
        UG_WindowSetTitleTextColor(image_window, C_RED);
        UG_WindowSetTitleInactiveColor(image_window,C_GRAY ) ;
        bmp_array[0] = &eye_bmp;
        bmp_array[1] = &green_bmp;
        img_array[0] = &eye_img;
        img_array[1] = &green_img;


        int image_count = 0;
        if (UG_WindowShow(image_window)) {
                ERROR("Window creation failure");
        }
        gui_update();

        while(1){
                nk_keycode_t key = nk_dequeue_keycode(nk_get_cur_vc());
                if(key != NO_KEY) {
                        switch (key) {
                        case 's':
                                /*//return_to_wm(get_cur_thread()); */
                                /* DEBUG("Waking Desktop\n"); */
                                /* nk_sched_awaken(get_desktop_thread(), CPU_ANY); */
                                /* DEBUG("woke up desktop\n"); */
                                /* nk_sched_sleep(); */
                                break;
                        case KEY_KPRIGHT:
                                next_image(image_window , image_count);
                                image_count++;
                                break;
                        case KEY_KPLEFT:
                                //TODO:call display image
                                image_count--;
                                prev_image(image_window, image_count);

                                break;
                        default:
                                break;
                        }
                        gui_update();
                }
        }

}

void image_browser_startup()
{
        INFO("Launching from thread %d\n", get_cur_thread());
        if(nk_thread_start(image_browser_go,
                           NULL,
                           NULL,
                           false,
                           0,
                           &image_browser_thread,
                           CPU_ANY)){
                ERROR("Couldn't start wm thread");
                return;
        }

        wm_add_app(image_browser_thread);
}


void cats_startup()
{
        nk_sched_awaken(get_desktop_thread(), CPU_ANY);
}
void memes_startup()
{
        nk_sched_awaken(get_desktop_thread(), CPU_ANY);
}
void slides_startup()
{
        image_browser_startup();
}
