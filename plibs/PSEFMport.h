#ifndef __PSEFMPORT_H
#define __PSEFMPORT_H

// based on FreeRTOS API

// servant operation
#define port_servant_create(servant_id, runnable) xTaskCreate(runnable, const signed portCHAR * const (servant_id + '0'), 128, NULL, 2, NULL) 
#define port_servant_yield() taskYIELD()


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

#endif
