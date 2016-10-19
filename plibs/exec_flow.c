#include "exec_flow.h"

ps_servant_t * pcurrent_servant;

void prv_ef_update_current_servant(id_t  servant_id)
{
    pcurrent_servant = servants[servant_id];
}


void prv_ef_set_dest( ps_event_t * pevent )
{

}
