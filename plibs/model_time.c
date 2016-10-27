#include "model_time.h"

static tick_t xModelTimeStart;
static tick_t units[NUMOFMODES];
extern ps_mode_array_t mod;

static tick_t GCD(tick_t a, tick_t b){
	return b==0?a:GCD(b,a%b);
}

static tick_t  prv_model_time_unit_length(ps_mode_t * pmode)
{
	int i;
	tick_t unit_len = pmode->tasks[0].period;
	for( i = 1; i < pmode->num; ++i){
		unit_len = GCD(unit_len, pmode->tasks[i].period);
	}
	return unit_len;
}

void prv_model_time_unit_initialize()
{
	int i;
	for( i = 0; i < mod.num; ++i){
		units[mod.pmodes[i]->mode_id] = prv_model_time_unit_length(mod.pmodes[i]);
	}
}

tick_t prv_model_time_input_length()
{
	return INPUT:
}

tick_t prv_model_time_output_length()
{
	return OUTPUT;
}


tick_t prv_model_time_unit_start()
{
	ps_mode_t * pmode = prv_mode_get_current_mode();
	id_t mode_id = pmode->mode_id;
	tick_t current_model_time = port_get_current_time();
	
	return current_model_time - (current_model_time - xModelTimeStart)%units[mode_id];  // return absolute time
}

void prv_model_time_reset()
{
	xModelTimeStart = port_get_current_time();
}

tick_t prv_model_time_get_model_time()
{
	return xModelTimeStart;
}

tick_t prv_model_time_intput_end()
{
	return prv_model_time_unit_start()+INPUT;
}

tick_t prv_model_time_output_end()
{
	ps_mode_t *pmode = prv_mode_get_current_mode();
	id_t mode_it = pmode->mode_id;
	return prv_model_time_unit_start()+units[mode_id];
}

tick_t prv_model_time_output_start()
{
	return prv_model_time_output_end()-OUTPUT;
}




