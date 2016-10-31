#define USE_STDPERIPH_DRIVER
#include "app.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "stm32f10x.h"

/*
   ps_servant_t * ps_servant_create(id_t servant_id, int servant_type, tick_t LED, int num, ps_servant_t *src_array[], void (*runnable)(void *));

   void ps_task_create(id_t task_id, tick_t LET, ps_servant_t * servants[],  int servant_num);

   void ps_mode_create(id_t mode_id, ps_task_t * task_array[], int num);

   void ps_mode_switch_create(bool (*condition)(void), id_t mode_dest)

 */

/******************************************************
Function:
Description:
Calls:
Called by:
Input:
Output:
Return:
Others:
 * *****************************************************/
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

	ps_servant_t * ps_servants[NUMOFSERVANTS];
	ps_task_t * ps_tasks[NUMOFTASKS];

	// parameters : servant_id , servant_type, LED, num of src, src_array[], runnable
	ps_servants[0] = ps_servant_create(0, 0, 20, 0, NULL, sensor1);
	ps_servants[1] = ps_servant_create(1, 1, 1000, 1, ps_servants, controller11);
	ps_servants[2] = ps_servant_create(2, 1, 500,  1, ps_servants, controller12);
	ps_servants[3] = ps_servant_create(3, 2, 20, 2, ps_servants+1, actuator1);


	ps_servants[4] = ps_servant_create(4, 0, 20, 0, NULL, sensor2);
	ps_servants[5] = ps_servant_create(5, 1, 2000, 1, ps_servants+4, controller21);
	ps_servants[6] = ps_servant_create(6, 1, 1500, 1, ps_servants+5, controller22);
	ps_servants[7] = ps_servant_create(7, 2, 20, 1, ps_servants+6, actuator2);


	// parameters: task_id, LET, servant_array[], servant_num
	ps_tasks[0] = ps_task_create(0, 5000, ps_servants, 4);
	ps_tasks[1] = ps_task_create(1, 10000,  ps_servants+4, 4);

	// parameters: mode_id, task_array[], task_num
	ps_mode_create(0, ps_tasks, 2);
	// parameters: condition, mode_dest
	ps_mode_switch_create( mode_switch1, 0);

	// PSEFM kernel initialize and RTOS scheduler start
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
	// scheduled at every tick interrupt, to trigger periodic task to run or trigger R-Servant to run
	ps_mode_switch();
}
