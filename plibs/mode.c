#include "mode.h"

struct ps_condition_array_t cond;
struct ps_mode_array_t mod;

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


// public API
void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num)
{
    int i;
	
    modes[mode_id].mode_id = mode_id;
    for(i=0;i<num;++i){
        prv_mode_add_task(mode_id, task_array[i]);
    }

	mod.pmodes[mod.num] = &modes[mode_id];
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
    for(i=0;i<cond.num;++i){
        if(cond.conditions[i].condition() == 1){
            ps_mode_start(cond.conditions[i].mode_dest);
			prv_model_time_reset();  // reset the xModeTimeStart
            break;
        }
    }
    if(i == cond.num){
        ps_mode_start(prv_mode_get_current_mode());
    }

	prv_event_future_model_time_reset();  // when enter new mode period, set the xFutureModelTime as the Input end.
}
