#ifndef __MODEL_TIME_H
#define __MODEL_TIME_H

#include "PSEFMconfigure.h"
#include "mode.h"

tick_t prv_model_time_unit_start();

bool prv_model_time_is_unit_start();

bool prv_model_time_is_mode_end();

tick_t prv_model_time_output_start();

tick_t prv_model_time_output_end();

tick_t prv_model_time_input_end();

tick_t prv_model_time_input_length();

tick_t prv_model_time_output_length();

void prv_model_time_unit_initialize();

void prv_model_time_period_initialize();

void prv_model_time_initialize();

void prv_model_time_reset();

tick_t prv_model_time_get_model_time();

bool prv_model_time_is_period_start(ps_task_t * ptask);



#endif
