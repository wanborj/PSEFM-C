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
	item_t * pevent_item = prv_list_get_first_item(&xEventLocalList);
	item_t * pevent_last = prv_list_get_last_item(&xEventLocalList);

	ps_servant_t * pservant;
	item_t * pevent_iterator;
	int num, i, j, len;

	len = prv_list_get_length(&xEventLocalList);
	for(i = 0 ; i < len; pevent_item = prv_item_get_next(pevent_item){

		pservant = prv_event_get_dest((ps_event_t *)pevent_item->item));
		num = prv_servant_get_num(pservant);
		if(prv_servant_get_arrive(pservant) == num){
			pevent_iterator = pevent_item;
			for(j=0; j < num && i < len; pevent_iterator = prv_item_get_next(pevent_iterator)){
				if(pservant == prv_event_get_dest((ps_event_t *)pevent_iterator->item)){

					prv_list_remove(pevent_item);
					prv_list_insert(pevent_iterator, &xEventReadyList);
					j ++;
				} //end if
				i ++;
			} // end for
			break;
		} //end if
	}// end for

	prv_servant_trigger(pservant);
}
