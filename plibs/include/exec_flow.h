#ifndef __EXEC_FLOW_H
#define __EXEC_FLOW_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "event.h"
#include "mode.h"
#include "ptask.h"
#include "servant.h"
#include "list_internal.h"

typedef struct relation{
    int num;
    ps_servant_t * pservants[NUMOFINS];
} ps_relation_t;

// maintain the global communication relationship

int prv_ef_get_dest_num(ps_servant_t * pservant);

ps_servant_t * prv_ef_get_ith_dest(ps_servant_t * pservant, int i);

void prv_ef_set_current_servant(ps_servant_t * pservant);

ps_servant_t * prv_ef_get_current_servant();

// build the communication relation, invoked in ps_servant_create()
void prv_ef_add_relation(ps_servant_t *, ps_servant_t *);


// DE model,
// transit events from global list to local list with determinate ordering
// transit events from local list to ready list for triggering next servant to run
void prv_ef_sorting();

void prv_ef_triggering();

void prv_ef_create();

#endif
