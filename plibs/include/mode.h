#ifndef __MODE_H
#define __MODE_H

#include "PSEFMconfigure.h"
#include "ptask.h"
#include "event.h"

struct ps_condition_array_t{
    int num;
    ps_mode_cond conditions[NUMOFCONDS];
};

struct ps_mode_array_t{
    int num;
    ps_mode_t * pmode[NUMOFMODES];
};

/******************************************************
Function: ps_mode_create
Description: Programming API, create mode according to the tasks in parameters
Calls:
Called by: main()
Input: mode_id(mark the mode for mode switch); task_array[](tasks the mode contains); num (number of tasks in the mode)
Output:
Return: void
Others:
 * *****************************************************/
void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num);


/******************************************************
Function: prv_mode_add_task
Description: kernel function, add one task into specific mode
Calls:
Called by: ps_mode_create()
Input: mode_id( id of mode ); ptask( the pointer of task )
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_add_task(id_t mode_id, ps_task_t * ptask);


/******************************************************
Function: prv_mode_get_mode_period
Description: kernel function, get the period of specified mode
Calls:
Called by: prv_model_time_is_mode_end()
Input: mode_id( id of mode )
Output:
Return: period of modes[mode_id]
Others:
 * *****************************************************/
tick_t  prv_mode_get_mode_period(id_t mode_id);


/******************************************************
Function: prv_mode_set_mode_period
Description: kernel function, set the period of specified mode
Calls:
Called by: prv_model_time_period_initialize()
Input: mode_id; period;
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_set_mode_period(id_t mode_id , tick_t period);


/******************************************************
Function: prv_mode_get_mode_unit
Description: kernel function, get the unit length of specified mode
Calls:
Called by: prv_model_time_unit_start();  prv_model_time_is_unit_start(); prv_model_time_output_end()
Input: mode_id
Output:
Return: the length of mode unit which is the GCD of periods of all tasks in the mode
Others:
 * *****************************************************/
tick_t prv_mode_get_mode_unit(id_t mode_id);


/******************************************************
Function: prv_mode_set_mode_unit
Description: kernel function, set the unit property of mode
Calls:
Called by: prv_model_time_unit_initialize()
Input: mode_id; unit(the unit length of the mode)
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_set_mode_unit(id_t mode_id, tick_t unit);

/******************************************************
Function: ps_mode_switch_create
Description: Programming API, create the relationship of the mode switch condition with the destination mode
Calls:
Called by: main()
Input: condition( to judge whether mode switch happended ); mode_dest( the target mode for this mode switch condition )
Output:
Return: void
Others:
 * *****************************************************/
void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest);


/******************************************************
Function: ps_mode_switch
Description: Programming API, run every tick interrupt happended.
Implemented the time-triggered semantics, triggered R-Servant and Servants to run according to the information of events
Calls:
Called by: vApplicationTickHook()
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_mode_switch();


/******************************************************
Function: prv_mode_start
Description: kernel function, triggered all the tasks in one mode to run when time reach the beginning of a mode
Calls: prv_task_start()
Called by: ps_mode_switch(); system_start()
Input: mode_id
Output:
Return: void
Others:
 * *****************************************************/
void prv_mode_start(id_t mode_id);

/******************************************************
Function: prv_mode_get_current_mode
Description: Kernel function, get the pointer of current mode which only may be changed when mode switch
Calls:
Called by: ps_mode_switch(); prv_model_time_unit_start(); prv_model_time_is_unit_start(); prv_model_time_output_end(); prv_model_time_is_mode_end();
Input:
Output:
Return: the pointer of current mode
Others:
 * *****************************************************/
ps_mode_t * prv_mode_get_current_mode();


/******************************************************
Function: prv_mode_get_task_num
Description: kernel function, get the number of tasks in one mode
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
int  prv_mode_get_task_num(id_t mode_id);

/******************************************************
Function: system_start
Description: Programming API, Initialize the model time, event list and execution flow, and start the RTOS scheduler
Calls:
Called by: main()
Input:
Output:
Return:
Others:
 * *****************************************************/
void system_start();


#endif
