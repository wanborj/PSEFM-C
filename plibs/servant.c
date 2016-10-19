#include "servant.h"

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

void      prv_servant_add_arrive(ps_servant_t *pservant)
{
    pservant->arrive ++;
}

/* create servant and record the time,relation and function information */
void ps_servant_create(id_t servant_id, int servant_type, tick_t LET, tick_t LED,
                                int num,
                                ps_servant_t *src_array,
                                void (*runnable)(void *),
                                ps_servant_t * pservant)
{
    int i;
    pservant->servant_id = servant_id;
    pservant->servant_type = servant_type;
    pservant->LET = LET;
    pservant->LED = LED;
    pservant->num = num;
    pservant->arrive = 0;

    //prv_thread_create();

    for(i = 0; i < num; ++ i){
        //prv_build_relation(src_array[i], pservant);
        //API from exec_flow
    }
}

void ps_servant_cooperate()
{
    prv_servant_yield();
    // yield API
}
