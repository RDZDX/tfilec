#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"
#include <time.h>
#include "stdio.h"
#include "string.h"
#include "stdint.h"

VMINT		layer_hdl[1];	/* layer handle array. */

void handle_sysevt(VMINT message, VMINT param);
VMINT cb(VMINT act, VMUINT32 total, VMUINT32 completed, VMINT hdl);
void create_auto_filename(VMWSTR text);
void create_auto_full_path_name(VMWSTR result, VMWSTR fname);
VMINT file(VMWCHAR *FILE_PATH, VMINT wlen);
VMINT file1(VMWCHAR *FILE_PATH, VMINT wlen);

#endif

