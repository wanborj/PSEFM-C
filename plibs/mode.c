#include "mode.h"

ps_mode_cond switch_conds[NUMOFCONDS];
ps_mode_t modes[NUMOFMODES];

void ps_mode_create(id_type mode_id, void(* runnable)(void *))
{
    (*runnable)((void *)&mode_id);
}

void ps_mode_start(id_type mode_id)
{
    /*start all the tasks in this mode*/
    int i;
    for(i = 0; i < modes[mode_id].num; ++i){
        prv_task_start(mode_id, i);
    }
}


void ps_mode_switch_create(id_type cond_id, bool (*condition)(void), id_type mode_dest)
{
    switch_conds[cond_id].mode_dest = mode_dest;
    switch_conds[cond_id].condition = condition;
}
