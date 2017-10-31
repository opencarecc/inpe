#ifndef _VMSLEEP_H_
#define _VMSLEEP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "vmpwr.h"
#include "vmgsm.h"
#include "vmgsm_gprs.h"
#include "vmbt_cm.h"
#include "vmgsm_cell.h"
#include "vmtimer.h"


boolean sleepIn(void* user_data);
boolean sleepOut(void* user_data);


#ifdef __cplusplus
}
#endif

#endif