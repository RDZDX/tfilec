#include "Tfilec.h"

VMINT trigeris = 1;
VMINT trigeris1 = 0;
VMINT trigeris2 = 0;
VMINT trigeris3 = 0;
VMWCHAR autoFullPathName[100];
VMUINT8 *buffer = NULL;

void vm_main(void) {
	layer_hdl[0] = -1;	
	vm_reg_sysevt_callback(handle_sysevt);
	vm_reg_keyboard_callback(handle_keyevt);
	vm_reg_pen_callback(handle_penevt);
        trigeris = vm_selector_run(0, 0, file);
}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            layer_hdl[0] =
                vm_graphic_create_layer(0, 0, vm_graphic_get_screen_width(),
                                        vm_graphic_get_screen_height(), -1);
            vm_graphic_set_clip(0, 0, vm_graphic_get_screen_width(),
                                vm_graphic_get_screen_height());
            buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
            //vm_switch_power_saving_mode(turn_off_mode);
            break;

        case VM_MSG_PAINT:
            //vm_switch_power_saving_mode(turn_off_mode);
            mre_draw_black_rectangle();
            if (trigeris3 == 1) {trigeris = vm_selector_run(0, 0, file1);}
            if (trigeris1 == 1) {trigeris2 = 1;}
            if (trigeris == 0) {trigeris1 = 1;}
            if (trigeris2 == 1) {vm_exit_app();}
            break;

        case VM_MSG_INACTIVE:

            //vm_switch_power_saving_mode(turn_on_mode);
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
}

void handle_keyevt(VMINT event, VMINT keycode) {

 if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_RIGHT_SOFTKEY) {
        if (layer_hdl[0] != -1) {
           vm_graphic_delete_layer(layer_hdl[0]);
           layer_hdl[0] = -1;
        }
        vm_exit_app();
    }

    if (event == VM_KEY_EVENT_UP && keycode == VM_KEY_LEFT_SOFTKEY) {
        trigeris = vm_selector_run(0, 0, file1);
    }
}

void handle_penevt(VMINT event, VMINT x, VMINT y)
{
	/* touch and return*/
	if( layer_hdl[0] != -1 )
	{
		vm_graphic_delete_layer(layer_hdl[0]);
		layer_hdl[0] = -1;
	}
	
	vm_exit_app();		/* quit application */
}

static void draw_hello(void) {

	VMWSTR s;					/* string's buffer */
	int x;						/* string's x coordinate */
	int y;						/* string's y coordinate */
	int wstr_len;				/* string's length */
	vm_graphic_color color;		/* use to set screen and text color */

	
	/* get string content from resource */
	s = (VMWSTR)vm_res_get_string(STR_ID_HELLO);
	
	/* calculate string length*/ 
	wstr_len = vm_graphic_get_string_width(s);

	/* calculate string's coordinate */
	x = (vm_graphic_get_screen_width() - wstr_len) / 2;
	y = (vm_graphic_get_screen_height() - vm_graphic_get_character_height()) / 2;
	
	/* set screen color */
	color.vm_color_565 = VM_COLOR_WHITE;
	vm_graphic_setcolor(&color);
	
	/* fill rect with screen color */
	vm_graphic_fill_rect_ex(layer_hdl[0], 0, 0, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	
	/* set text color */
	color.vm_color_565 = VM_COLOR_BLUE;
	vm_graphic_setcolor(&color);
	
	/* output text to layer */
	vm_graphic_textout_to_layer(layer_hdl[0],x, y, s, wstr_len);
	
	/* flush the screen with text data */
	vm_graphic_flush_layer(layer_hdl, 1);
}

VMINT cb(VMINT act, VMUINT32 total, VMUINT32 completed, VMINT hdl){}

void create_auto_filename(VMWSTR text) {

    struct vm_time_t curr_time;
    VMCHAR fAutoFileName[100];

    vm_get_time(&curr_time);
    sprintf(fAutoFileName, "%02d%02d%02d%02d%02d.txt", curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
    vm_ascii_to_ucs2(text, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);

}

void create_auto_full_path_name(VMWSTR result, VMWSTR fname) {

    VMINT drv;
    VMCHAR fAutoFileName[100];
    VMWCHAR wAutoFileName[100];

    if ((drv = vm_get_removable_driver()) < 0) {
       drv = vm_get_system_driver();
    }

    sprintf(fAutoFileName, "%c:\\", drv);
    vm_ascii_to_ucs2(wAutoFileName, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);
    vm_wstrcat(wAutoFileName, fname);
    vm_wstrcpy(result, wAutoFileName);

}

void mre_draw_black_rectangle(void) {

    buffer = vm_graphic_get_layer_buffer(layer_hdl[0]);
    vm_graphic_fill_rect(buffer, 0, 0, vm_graphic_get_screen_width(),
                         vm_graphic_get_screen_height(), VM_COLOR_BLACK,
                         VM_COLOR_BLACK);
    vm_graphic_flush_layer(layer_hdl, 1);
}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;
    VMWCHAR autoFileName[100];
    create_auto_filename(autoFileName);
    create_auto_full_path_name(autoFullPathName, autoFileName);
    vm_file_copy(autoFullPathName, FILE_PATH, cb);
    FILE_PATH = NULL;
    //mre_draw_black_rectangle();
    //trigeris = vm_selector_run(0, 0, file1);
    trigeris3 = 1;
    return 0;
}

VMINT file1(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris3 = 0;
    trigeris = 1;
    trigeris1 = 0;
    trigeris2 = 0;
    VMCHAR new_data[2000];
    VMUINT nread;
    VMFILE f_read;
    //FILE_PATH = NULL;
    //vm_exit_app();

   f_read = vm_file_open(FILE_PATH, MODE_READ, FALSE);
   vm_file_read(f_read, new_data, 2000, &nread);
   new_data[nread] = '\0';
   vm_file_close(f_read);
   f_read = vm_file_open(autoFullPathName, MODE_APPEND, FALSE);
   vm_file_write(f_read, new_data, strlen(new_data), &nread);
   vm_file_close(f_read);
   trigeris2 = 1;
   return 0;
}

/*
   while ((c = fgetc(fp1)) != EOF) 
   while ((ch = fgetc(fs1)) != EOF)
   fputc(ch,ft);
*/