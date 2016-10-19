#ifndef __EXEC_FLOW_H
#define __EXEC_FLOW_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"

void prv_ef_update_current_servant(id_t servant_id);

void prv_ef_set_dest( ps_event_t * pevent );

#endif
