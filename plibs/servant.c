#include "servant.h"
#include "exec_flow.h"
#include "PSEFMconfigure.h"

extern ps_event_sem_t sem[NUMOFSERVANTS];

ps_servant_t * pservants[NUMOFSERVANTS];

id_t      prv_servant_get_id(ps_servant_t *pservant)
{
    return pservant->servant_id;
}


int prv_servant_get_type(ps_servant_t * pservant)
{
    return pservant->servant_type;
}

tick_t    prv_servant_get_LED(ps_servant_t *pservant)
{
    return pservant->LED;
}

tick_t    prv_servant_get_LET(ps_servant_t *pservant)
{
    return pservant->LET;
}

tick_t prv_servant_get_start_time(ps_servant_t * pservant)
{
    return pservant->start_time;
}

int  prv_servant_get_num(ps_servant_t *pservant)
{
    return pservant->num;
}

int  prv_servant_get_arrive(ps_servant_t *pservant)
{
    return pservant->arrive;
}

void prv_servant_set_arrive( ps_servant_t * pservant, int n)
{
	pservant->arrive = n;
}

void prv_servant_clean_arrive( ps_servant_t * pservant)
{
	prv_servant_set_arrive(pservant, 0);
}

void prv_servant_add_arrive(ps_servant_t *pservant)
{
    pservant->arrive ++;
}

void prv_servant_trigger( ps_servant_t * pservant)
{
	id_t servant_id = prv_servant_get_id( pservant );
	prv_ef_set_current_servant(pservant);  // mark the dest servant as the current servant
	port_trigger(sem[servant_id]);  // trigger the sem of the dest servant
}


/* create servant and record the time,relation and function information */
ps_servant_t * ps_servant_create(id_t servant_id, int servant_type, tick_t LED,
                                int num,
                                ps_servant_t *src_array[],
                                void (*runnable)(void *))
{
    int i;
    ps_servant_t * pservant = (ps_servant_t *)port_malloc(sizeof(ps_servant_t));

    pservant->servant_id = servant_id;
    pservant->servant_type = servant_type;
    pservant->LED = LED;
    pservant->num = num;
    pservant->arrive = 0;

    pservants[servant_id] = pservant; // store the new servant into servant array
    port_servant_create(runnable, 2);

    for(i = 0; i < num; ++ i){
        prv_ef_add_relation(src_array[i], pservant);
    }

	return pservant;
}

void ps_servant_cooperate()
{
    port_trigger(sem[NUMOFSERVANTS-1]);
    port_servant_yield();
    // yield API
}


