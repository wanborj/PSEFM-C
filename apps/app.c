#include "app.h"

void sensor1( void * para )
{
	while(1){
        // blocking for periodic timing event arriving
		ps_event_wait(para);

		// process pevent including tag and data
		port_print("sensor 1\n\r");

        // parameter :pointer of struct data
        ps_event_create(NULL);

        // release CPU and trigger R-Sevant to execute
		ps_servant_cooperate();
	}
}

void controller1(void * para)
{
	ps_data_t * data;

	while(1){
        // blocking for message-arrival event arriving
		ps_event_wait(para);
        // receive ready event from xEventReadyList
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 1\n\r");

        // parameter one is not NULL mean that sysmem will upate the existing event
        ps_event_create(data);
		ps_servant_cooperate();
	}
}

void actuator1(void * para)
{
	ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 1\n\r");

        // output the event's data to physical device
		ps_servant_cooperate();
	}
}

void sensor2(void * para)
{
	while(1){
		ps_event_wait(para);

		// process pevent including tag and data
		port_print("sensor 2\n\r");

		ps_event_create(NULL);
		ps_servant_cooperate();
	}

}

void controller2(void * para)
{
    ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 2\n\r");

		ps_event_create(NULL);
		ps_servant_cooperate();
	}

}

void actuator2(void * para)
{
    ps_data_t * data;

	while(1){
		ps_event_wait(para);
		data = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 2\n\r");
        // output the event's data to physical device

		ps_servant_cooperate();
	}
}




