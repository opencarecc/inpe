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

#include "arduino.h"
#include "vmlog.h"
#include "vmfs.h"
#include "vmchset.h"
#include <stdio.h>
#include <string.h>
#include "CFile.h"


boolean file_ext_create(void* user_data)
{
  file_info_struct* data_file = (file_info_struct*)user_data;
  
  VMCHAR filename[VM_FS_MAX_PATH_LENGTH] = {0};
  VMWCHAR wfilename[VM_FS_MAX_PATH_LENGTH] = {0};
  VM_FS_HANDLE filehandle = -1;

  sprintf((char*)filename, "%c:\\%s", vm_fs_get_internal_drive_letter(), data_file->filename);
  vm_chset_ascii_to_ucs2(wfilename, sizeof(wfilename), filename);

    if((filehandle = vm_fs_open(wfilename, VM_FS_MODE_CREATE_ALWAYS_WRITE, TRUE)) < 0)
    {
        vm_log_info("Failed to create file: %s",filename);
    //Serial1.print("Failed to create file.\r\n");
        return true;
    }
    vm_log_info("Success to create file: %s", filename);
    data_file->handle = filehandle;
  return true;
}

boolean file_ext_write(void* user_data)
{
  file_info_struct* data_file = (file_info_struct*)user_data;
  
  VMUINT writelen = 0;
  VMINT ret = 0;
  ret = vm_fs_write(data_file->handle, (void*)data_file->data, strlen(data_file->data), &writelen);
    if(ret < 0)
    {
        vm_log_info("Failed to write file");
        return true;
    }
  return true;
}

boolean file_ext_close(void* user_data)
{
  file_info_struct* data_file = (file_info_struct*)user_data;
  vm_fs_close(data_file->handle);
  return true;
}
