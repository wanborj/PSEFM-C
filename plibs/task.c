#include "task.h"

struct ps_task_array_t tsk;


id_t   prv_task_get_id(ps_task_t * ptask)
{
    return ptask->task_id;
}


tick_t prv_task_get_LET(ps_task_t * ptask)
{
    return ptask->LET;
}

tick_t prv_task_get_period(ps_task_t * ptask)
{
    return ptask->period;
}

tick_t prv_task_get_deadline(ps_task_t * ptask)
{
    return ptask->deadline;
}

int    prv_task_get_servant_num(ps_task_t * ptask)
{
    return ptask->servant_num;
}

ps_servant_t * prv_task_get_sensor(ps_task_t * ptask)
{
    return ptask->servants[0];
}

ps_servant_t * prv_task_get_actuator(ps_task_t * ptask)
{
    return ptask->servants[ptask->servant_num - 1];
}

void   prv_task_start(ps_task_t * ptask)
{
    prv_servant_trigger( prv_task_get_sensor(ptask) );
}

ps_task_t * ps_task_create(id_t task_id, tick_t LET,
                       ps_servant_t * servants[],
                       int servant_num)
{
    int i, pos = tsk.num;
    tsk.tasks[pos].task_id = task_id;
    tsk.tasks[pos].LET = tsk.tasks[pos].period = tsk.tasks[pos].deadline = LET;
    tsk.tasks[pos].servant_num = servant_num;

    for(i=0;i<servant_num;++i){
        tsk.tasks[pos].servants[i] = servants[i];
        tsk.tasks[pos].servants[i]->LET = LET;
    }
    tsk.num ++;

	return &tsk.task[pos];
}
