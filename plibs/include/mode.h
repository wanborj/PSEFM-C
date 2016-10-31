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
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
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
Called by:
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
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_mode_set_mode_period(id_t mode_id , tick_t period);


/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
tick_t prv_mode_get_mode_unit(id_t mode_id);


/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void prv_mode_set_mode_unit(id_t mode_id, tick_t unit);

/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest);


/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_mode_switch();


/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void ps_mode_start(id_t mode_id);

/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
ps_mode_t * prv_mode_get_current_mode();


/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
int  prv_mode_get_task_num(id_t mode_id);

/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
void system_start();


#endif
