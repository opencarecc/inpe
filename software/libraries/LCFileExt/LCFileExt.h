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


#ifndef _LCFILEEXT_H
#define _LCFILEEXT_H

#include "LTask.h"
#include "utility/CFile.h"

class LCFileExtClass  : public _LTaskClass {

public:
  LCFileExtClass() { };

public:
  void Create(const char* filename);
    
  void Write(const char* strbuf);
  
  void Close();

private:
  file_info_struct file;
};

extern LCFileExtClass LFileExt;

#endif
