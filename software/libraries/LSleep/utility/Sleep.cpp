#include "LSleep.h"
#include "Sleep.h"

unsigned char sleepStatus = 0;

void sys_sleep_in_callback(VM_TIMER_ID_PRECISE tid, void* user_data)
{
    if(sleepStatus == 0)
    {
        Serial.println("sys_sleep_in_callback");
        sleepStatus = 1;
        vm_pwr_lcd_sleep_in();
        vm_gsm_cell_close();
        vm_gsm_switch_mode(0, NULL);
        vm_gsm_gprs_switch_mode(0);
        vm_bt_cm_switch_off();       
    }
    vm_timer_delete_non_precise(tid);
}

void sys_sleep_out_callback(VM_TIMER_ID_PRECISE tid, void* user_data)
{
    if(sleepStatus == 1)
    {
        Serial.println("sys_sleep_out_callback");
        sleepStatus = 0;
        vm_pwr_lcd_sleep_out();
        vm_gsm_cell_open();
        vm_gsm_switch_mode(1, NULL);
        vm_gsm_gprs_switch_mode(1);
        vm_bt_cm_switch_on();    
    }
    vm_timer_delete_non_precise(tid);  
}

boolean sleepIn(void* user_data)
{
    vm_timer_create_non_precise(100, sys_sleep_in_callback, NULL);
}

boolean sleepOut(void* user_data)
{
    vm_timer_create_non_precise(100, sys_sleep_out_callback, NULL);
}