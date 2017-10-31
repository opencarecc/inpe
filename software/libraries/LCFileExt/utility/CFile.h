/*
  Copyright (c) 2014 MediaTek Inc.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License..

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
   See the GNU Lesser General Public License for more details.
*/


#ifndef _CFILE_H
#define _CFILE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "vmfs.h"

boolean file_ext_create(void* user_data);
boolean file_ext_write(void* user_data);
boolean file_ext_close(void* user_data);

typedef struct
{
  char filename[256];
  const char *data;
  VM_FS_HANDLE handle;
}
file_info_struct;

#ifdef __cplusplus
}
#endif

#endif

