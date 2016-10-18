#ifndef __TASK_H
#define __TASK_H

#include "PSEFMconfigure.h"
#include "servant.h"

typedef struct task{
    id_t task_id;
    tick_t LET;
    tick_t period;
    tick_t deadline;
    int servant_num;      // number of I-servants
    ps_servant_t * servants[NUMOFSERVANTS];  // I-servants in this task
}ps_task_t;

struct ps_task_array_t{
    int num;
    ps_task_t tasks[NUMOFTASKS*NUMOFMODES];
};

extern struct ps_task_array_t tsk;

id_t   prv_task_get_id(ps_task_t *);
tick_t prv_task_get_LET(ps_task_t *);
tick_t prv_task_get_period(ps_task_t *);
tick_t prv_task_get_deadline(ps_task_t *);
int    prv_task_get_servant_num(ps_task_t *);
void   prv_task_start(ps_task_t * ptask);

ps_servant_t * prv_task_get_sensor(ps_task_t *);
ps_servant_t * prv_task_get_actuator(ps_task_t *);


/* create task and run its runnable function to create servants */
void ps_task_create(id_t task_id,  // start from zero
                    tick_t LET,
                    int servant_num,
                    ps_servant_t * servants[]);


#endif

