#include "Tfilec.h"

VMINT trigeris = 1;
VMBOOL trigeris1 = VM_FALSE;
VMBOOL trigeris2 = VM_FALSE;
VMBOOL trigeris3 = VM_FALSE;
VMWCHAR autoFullPathName[100];
VMWCHAR selectedFilePath[100];
VMUINT8 *buffer = NULL;

void vm_main(void) {
	layer_hdl[0] = -1;	
	vm_reg_sysevt_callback(handle_sysevt);
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
            break;

        case VM_MSG_PAINT:
            if (trigeris3 == VM_TRUE) {trigeris = vm_selector_run(0, 0, file1);}
            if (trigeris1 == VM_TRUE) {trigeris2 = VM_TRUE;}
            if (trigeris == 0) {trigeris1 = VM_TRUE;}
            if (trigeris2 == VM_TRUE) {vm_exit_app();}
            break;

        case VM_MSG_INACTIVE:

            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;

        case VM_MSG_QUIT:
            if (layer_hdl[0] != -1) vm_graphic_delete_layer(layer_hdl[0]);
            break;
    }
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

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = VM_FALSE;
    trigeris2 = VM_FALSE;
    VMWCHAR autoFileName[100];
    create_auto_filename(autoFileName);
    create_auto_full_path_name(autoFullPathName, autoFileName);
    vm_wstrcpy(selectedFilePath, FILE_PATH);
    FILE_PATH = NULL;
    trigeris3 = VM_TRUE;
    return 0;
}

VMINT file1(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris3 = VM_FALSE;
    trigeris = 1;
    trigeris1 = VM_FALSE;
    trigeris2 = VM_FALSE;
    VMCHAR new_data[2000];
    VMUINT nread;
    VMFILE f_read;
    //VMFILE f_write;
    VMINT ret;

    vm_file_copy(autoFullPathName, selectedFilePath, cb);
    f_read = vm_file_open(FILE_PATH, MODE_READ, FALSE);
    ret = vm_file_read(f_read, new_data, 2000, &nread);

    //while (ret != 0) {
      //ret = vm_file_read(f_read, new_data, 2000, &nread);
      //vm_file_write(f_write, new_data, strlen(new_data), &nread);
    //}

    new_data[nread] = '\0';
    vm_file_close(f_read);
    f_read = vm_file_open(autoFullPathName, MODE_APPEND, FALSE);
    vm_file_write(f_read, new_data, strlen(new_data), &nread);
    vm_file_close(f_read);
    trigeris2 = VM_TRUE;
    return 0;
}
