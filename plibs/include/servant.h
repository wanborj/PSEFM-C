#ifndef __SERVANT_H
#define __SERVANT_H

#include "PSEFMconfigure.h"
#include "PSEFMport.h"
#include "exec_flow.h"


/*get set function*/
id_t      prv_servant_get_id(ps_servant_t *pservant);
tick_t    prv_servant_get_start_time(ps_servant_t * pservant);
tick_t    prv_servant_get_LED(ps_servant_t *pservant);
tick_t    prv_servant_get_LET(ps_servant_t *pservant);
int       prv_servant_get_type(ps_servant_t * pservant);
int       prv_servant_get_num(ps_servant_t *pservant);
int       prv_servant_get_arrive(ps_servant_t *pservant);

void      prv_servant_add_arrive(ps_servant_t *pservant);



/*
 * Function:
 * 1) record the information of a servant
 * 2) create the platform dependent thread
 * 3) build the communication relation
 * */
ps_servant_t * ps_servant_create(id_t servant_id,
                        int servant_type,
                        tick_t LED,
                        int num,   // number of elements in src_array
                        ps_servant_t *src_array,
                        void (*runnable)(void *));

void ps_servant_coorperate();  // invoked in runnable function in ps_servant_create API


#endif
