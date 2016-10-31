#include "mode.h"

struct ps_condition_array_t cond;
struct ps_mode_array_t mod = {0, NULL};
extern list_t xEventGlobalList;
extern ps_event_sem_t sem[NUMOFSERVANTS];

ps_mode_t modes[NUMOFMODES];

static ps_mode_t * current_mode;

void prv_mode_add_task(id_t mode_id, ps_task_t * ptask)
{
    int num = modes[mode_id].num;
    modes[mode_id].tasks[num] = ptask;
    modes[mode_id].num = num + 1;
}

ps_mode_t * prv_mode_get_current_mode()
{
    return current_mode;
}

int prv_mode_get_task_num(id_t mode_id)
{
    return modes[mode_id].num;
}

// set function implemented in model_time
tick_t prv_mode_get_mode_period(id_t mode_id)
{
    return modes[mode_id].period;
}

void prv_mode_set_mode_period(id_t mode_id , tick_t period)
{
    modes[mode_id].period = period;
}

tick_t prv_mode_get_mode_unit(id_t mode_id)
{
    return modes[mode_id].unit;
}

void prv_mode_set_mode_unit(id_t mode_id, tick_t unit)
{
    modes[mode_id].unit  = unit;
}


// public API
void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num)
{
    int i;

    modes[mode_id].mode_id = mode_id;
    for(i=0;i<num;++i){
        prv_mode_add_task(mode_id, task_array[i]);
    }

    mod.pmode[mod.num] = &modes[mode_id];
    mod.num ++;

}

void ps_mode_start(id_t mode_id)
{
    /*start all the tasks in this mode*/
    int i;
    current_mode = &modes[mode_id];
    for(i=0;i<modes[mode_id].num;++i){
        prv_task_start( modes[mode_id].tasks[i]);
    }
}


void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest)
{
    int num = cond.num;
    cond.conditions[num].mode_dest = mode_dest;
    cond.conditions[num].condition = condition;
    cond.num = num + 1;
}

void ps_mode_switch()
{
    int i;
    ps_mode_t * current_mode;
    ps_task_t  * ptask;

    if( prv_model_time_is_mode_end() == 1){  // start a new mode, and trigger all the tasks in this mode

        current_mode = prv_mode_get_current_mode();
        for(i=0;i<cond.num;++i){
            if(cond.conditions[i].condition() == 1){
                if(cond.conditions[i].mode_dest != current_mode->mode_id){
                    current_mode = &modes[cond.conditions[i].mode_dest];  //  set the mode as current mode
                    prv_model_time_reset();  // reset the xModeTimeStart only when mode switches

                }
                ps_mode_start(cond.conditions[i].mode_dest);
                break;
            }
        }
        if(i == cond.num){
            ps_mode_start(current_mode->mode_id);
        }
        prv_event_future_model_time_reset();  // when enter new mode period, set the xFutureModelTime as the Input end.

    }else if( prv_model_time_is_unit_start() == 1 ){ // start the task which finish their last periods

        current_mode = prv_mode_get_current_mode();
        for(i = 0; i < modes[current_mode->mode_id].num; ++i){
            ptask = modes[current_mode->mode_id].tasks[i];
            if(1 == prv_model_time_is_period_start(ptask)){
                prv_task_start( ptask);
            }
        }

    }else if( xEventGlobalList.earliest_time <= port_get_current_time() ){ // trigger R-Servant to run to process the events in list

        port_trigger( sem[NUMOFSERVANTS-1] );

    }else{
        // do nothing
    }
}



void system_start()
{
    prv_model_time_initialize();
    prv_event_list_initialize();
    prv_ef_create();
    ps_mode_start(0);
    port_scheduler_start();
}
