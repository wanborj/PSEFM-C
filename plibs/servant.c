#include "servant.h"
#include "exec_flow.h"

ps_servant_t * pservants[NUMOFSERVANTS];

id_t      prv_servant_get_id(ps_servant_t *pservant)
{
    return pservant->servant_id;
}

servant_t prv_servant_get_type(ps_servant_t * pservant)
{
    return pservant->servant_type;
}

tick_t    prv_servant_get_LED(ps_servant_t *pservant)
{
    return pservant->LED;
}

tick_t    prv_servant_get_LET(ps_servant_t *pservant)
{
    reutrn pservant->LET;
}

int       prv_servant_get_num(ps_servant_t *pservant)
{
    return pservant->num;
}

int       prv_servant_get_arrive(ps_servant_t *pservant)
{
    return pservant->arrive;
}

void 	  prv_servant_set_arrive( ps_servant_t * pservant, int n)
{
	pservant->arrive = n;
}

void 	prv_servant_clean_arrive( ps_servant_t * pservant)
{
	prv_servant_set_arrive(pservant, 0);
}

void      prv_servant_add_arrive(ps_servant_t *pservant)
{
    pservant->arrive ++;
}

/* create servant and record the time,relation and function information */
ps_servant_t * ps_servant_create(id_t servant_id, int servant_type, tick_t LED,
                                int num,
                                ps_servant_t *src_array,
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
    port_servant_create(servant_id, runnable);

    for(i = 0; i < num; ++ i){
        prv_ef_add_relation(src_array[i], pservant);
    }
}

void ps_servant_cooperate()
{
    port_servant_yield();
    // yield API
}

void prv_servant_trigger( ps_servant_t * pservant)
{
	id_t servant_id = prv_servant_get_id( pservant );
	pcurrent_servant = pservant;   // mark the dest servant as the current servant
	port_trigger(sem[servant_id]);  // trigger the sem of the dest servant
}
