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
#include "CFile.h"
#include "LCFileExt.h"


void LCFileExtClass::Create(const char* filename)
{
  strcpy(file.filename, filename);
  remoteCall(file_ext_create, &file);
}

void LCFileExtClass::Write(const char* strbuf)
{
  file.data = strbuf;
  remoteCall(file_ext_write, &file);
}

void LCFileExtClass::Close()
{
  remoteCall(file_ext_close, &file);
}

LCFileExtClass LFileExt;