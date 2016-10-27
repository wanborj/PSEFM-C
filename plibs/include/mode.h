#ifndef __MODE_H
#define __MODE_H

#include "PSEFMconfigure.h"
#include "task.h"
#include "event.h"



struct ps_condition_array_t{
    int num;
    ps_mode_cond conditions[NUMOFCONDS];
};

struct ps_mode_array_t{
    int num;
    ps_mode_t * pmode[NUMOFMODES];
};


extern ps_mode_t modes[NUMOFMODES];
extern ps_condition_array_t cond;

// mode setting
void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num);
void prv_mode_add_task(id_t mode_id, ps_task_t * ptask);
tick_t  prv_mode_get_mode_period(id_t mode_id);
void prv_mode_set_mode_period(id_t mode_id , tick_t period);
tick_t prv_mode_get_mode_unit(id_t mode_id);
void prv_mode_set_mode_unit(id_t mode_id, tick_t unit);

// mode switch setting
void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest);
void ps_mode_switch();
void ps_mode_start(id_t mode_id);
//void prv_mode_start_task(id_t mode_id, ps_task_t * ptask);

// get information
ps_mode_t * prv_mode_get_current_mode();
int  prv_mode_get_task_num(id_t mode_id);

// system init and RTOS scheduler start
void system_start();


#endif
