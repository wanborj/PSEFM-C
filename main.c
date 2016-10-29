#define USE_STDPERIPH_DRIVER
#include "app.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "stm32f10x.h"

/*
   ps_servant_t * ps_servant_create(id_t servant_id, int servant_type, tick_t LED,
   int num,
   ps_servant_t *src_array[],
   void (*runnable)(void *));

   void ps_task_create(id_t task_id, tick_t LET, ps_servant_t * servants[],  int servant_num);

   void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num);

   void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest)


 */
bool mode_switch1()
{
	return 1;
}

void myHardwareInit(){
	init_led();
	init_rs232();
	enable_rs232_interrupts();
	enable_rs232();
}


int main()
{
	myHardwareInit();
	port_print("I'm in main()\n\r");
	ps_servant_t * ps_servants[NUMOFSERVANTS];
	ps_task_t * ps_tasks[NUMOFTASKS];

	ps_servants[0] = ps_servant_create(0, 0, 3, 0, NULL, sensor1);
	ps_servants[1] = ps_servant_create(1, 1, 5, 1, ps_servants, controller1);
	ps_servants[2] = ps_servant_create(2, 2, 3, 1, ps_servants+1, actuator1);


	ps_servants[3] = ps_servant_create(3, 0, 3, 0, NULL, sensor2);
	ps_servants[4] = ps_servant_create(4, 1, 5, 1, ps_servants+3, controller2);
	ps_servants[5] = ps_servant_create(5, 2, 3, 1, ps_servants+4, actuator2);


	ps_tasks[0] = ps_task_create(0, 50, ps_servants, 3);
	ps_tasks[1] = ps_task_create(1, 100,  ps_servants+3, 3);

	ps_mode_create(0, ps_tasks, 2);
	ps_mode_switch_create( mode_switch1, 0);
	system_start();

}

void myTraceCreate      (){
}

void myTraceSwitchedIn  (){
}

void myTraceSwitchedOut	(){
}


void vApplicationTickHook( void )
{
	ps_mode_switch();
}
