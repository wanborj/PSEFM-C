#ifndef __PSEFMPORT_H
#define __PSEFMPORT_H

// based on FreeRTOS API

// servant operation
#define port_servant_create(runnable, prio) xTaskCreate(runnable, NULL, 128, NULL, prio, NULL) 

// trigger R_Servant to run
#define port_servant_yield() xSemaphoreGive(sem[NUMOFSERVANTS-1]) ; taskYIELD()


// event operation
#define port_wait(e)   xSemaphoreTake( e, portMAX_DELAY)
#define port_trigger(e) xSemaphoreGive(e)
// create semaphore and take the token away
#define port_pure_event_create(e)  xSemaphoreCreateBinary(e); \
									xSemaphoreTake(e, portMAX_DELAY) 
#define ps_event_sem_t xQueueHandle  


// get current information
#define port_get_current_time() xTaskGetTickCount()

// prv_ef_get_current_servant()

#define port_malloc(size) pvPortMalloc(size)

#define port_print(string)  { int i = 0; while( string[i]!='\0') { send_byte(string[i]; i++;}}

#define port_scheduler_start() vTaskStartScheduler()

#endif
