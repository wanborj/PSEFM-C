#ifndef __MODE_H
#define __MODE_H

#include "PSEFMconfigure.h"
#include "task.h"

typedef struct mode{
    int num;  // number of tasks
    ps_task_t tasks[NUMOFTASKS];
}ps_mode_t;

typedef struct cond{
    id_type mode_dest;
    bool (*condition)(void);
}ps_mode_cond;

void ps_mode_create(id_type mode_id, void (* runnable)(void *));

void ps_mode_start(id_type mode_id);

void ps_mode_switch_create(id_type cond_id, bool (*condition)(void), id_type mode_dest);

#endif
