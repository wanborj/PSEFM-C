#include "exec_flow.h"

static ps_servant_t * pcurrent_servant;
extern ps_servant_t * pservants[NUMOFSERVANTS];

ps_relation_t srcs[NUMOFSERVANTS];
ps_relation_t dests[NUMOFSERVANTS];


int prv_ef_get_dest_num(ps_servant_t * pservant)
{
    return dests[pservant->servant_id].num;
}

ps_servant_t * prv_ef_get_ith_dest(ps_servant_t * pservant, int i)
{
    return dests[pservant->servant_id].pservants[i];
}

void prv_ef_set_current_servant(ps_servant_t * pservant)
{
    pcurrent_servant = pservant;
}

ps_servant_t * prv_ef_get_current_servant()
{
    return pcurrent_servant;
}

void prv_ef_add_relation(ps_servant_t * pservant_src, ps_servant_t * pservant_dest)
{
    srcs[pservant_src->servant_id].pservants[num] = pservant_dest;
    srcs[pservant_src->servant_id].num ++;

    dests[pservant_dest->servant_id].pservants[num] = pservant_src;
    dests[pservant_dest->servant_id].num ++;
}

// R-Servant
void prv_ef_sorting()
{

}

void prv_ef_triggering()
{

}
