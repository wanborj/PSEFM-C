#include "exec_flow.h"

static ps_servant_t * pcurrent_servant;
extern ps_servant_t * pservants[NUMOFSERVANTS];
extern list_t xEventGlobalList;
extern list_t xEventReadyList;
extern list_t xEventLocalList;
extern ps_event_sem_t sem[NUMOFSERVANTS];


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
    int num = dests[pservant_src->servant_id].num;
    dests[pservant_src->servant_id].pservants[num] = pservant_dest;
    dests[pservant_src->servant_id].num ++;

    num = srcs[pservant_dest->servant_id].num;
    srcs[pservant_dest->servant_id].pservants[num] = pservant_src;
    srcs[pservant_dest->servant_id].num ++;

    pservant_dest->num ++; // the number of source servants
}

// R-Servant
void prv_ef_sorting()
{
    tick_t pcurrent_time = port_get_current_time();

    // sort the executable events
    item_t * pevent_item = prv_list_get_first_item( &xEventGlobalList );
    for(; pevent_item != prv_list_get_last_item( &xEventGlobalList );
            pevent_item = prv_item_get_next( pevent_item)){
        if(prv_event_get_timestamp((ps_event_t *) pevent_item->item) < pcurrent_time){
            prv_list_remove(pevent_item);
            prv_list_insert_sorted( pevent_item, &xEventLocalList);

            // add the arrive record in dest servant
            prv_servant_add_arrive( prv_event_get_dest((ps_event_t *)pevent_item->item ));
        }
    }
    prv_list_earlist_time_update( &xEventGlobalList);
}


void prv_ef_triggering()
{
    item_t * pevent_item, *pevent_temp, *pevent_iterator;
    ps_servant_t * pservant;
    int src_num, i, j, len, flag;

    len = prv_list_get_length(&xEventLocalList);
    pevent_item = prv_list_get_first_item(&xEventLocalList);

    for( i = 0 ; i < len;  i++){

        pservant = prv_event_get_dest((ps_event_t *)&pevent_item->item);
        src_num = prv_servant_get_num(pservant);
        if(prv_servant_get_arrive(pservant) == src_num){
            flag = prv_event_can_process((ps_event_t *)&pevent_item->item);
            if(2 == flag){
                // if events are I/O events

                pevent_temp = pevent_item;
                pevent_item = prv_item_get_next(pevent_item);
                prv_list_remove(pevent_item);
                prv_list_insert(pevent_item, &xEventReadyList);

                prv_servant_trigger(pservant);
                break;

            } else if( 1 == flag){
                // if events are executable controller events, then process the events with same dest servant

                prv_servant_clean_arrive(pservant);  // set the arrive of pservant to 0
                pevent_iterator = pevent_item;
                for(j=0; j < src_num && i < len;){
                    if(pservant == prv_event_get_dest((ps_event_t *)pevent_iterator->item)){
                        // send events to ready list
                        pevent_temp = pevent_iterator;
                        pevent_iterator = prv_item_get_next(pevent_iterator);
                        prv_list_remove(pevent_iterator);
                        prv_list_insert(pevent_iterator, &xEventReadyList);
                        j ++;
                    } //end if
                    i ++;
                } // end for
                prv_servant_trigger(pservant);

                break;
            }else{

                // transit all the left event in local list into the global event list

                pevent_temp = pevent_item;
                pevent_item = prv_item_get_next(pevent_item);
                prv_list_remove(pevent_temp);
                prv_list_insert(pevent_temp,&xEventGlobalList);
            }

        }
        else{
            pevent_item = prv_item_get_next(pevent_item);
        }
    }// end for

}


static void R_Servant()
{
    while(1){
        //port_wait(sem[NUMOFSERVANTS-1]);

        prv_ef_sorting();
        prv_ef_triggering();
    }
}

void prv_ef_create()
{
    port_servant_create(R_Servant, 1);
}
