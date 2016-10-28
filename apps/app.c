#include "app.h"

void sensor1()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("sensor 1\n\r");

		ps_event_create(pevent, NULL);
		ps_servant_cooperate();
	}
}

void controller1()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 1\n\r");

		ps_event_create(pevent, NULL);
		ps_servant_cooperate();
	}
}

void actuator1()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 1\n\r");

        // output the event's data to physical device
		ps_servant_cooperate();
	}
}

void sensor2()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("sensor 2\n\r");

		ps_event_create(pevent, NULL);
		ps_servant_cooperate();
	}

}

void controller2()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("controller 2\n\r");

		ps_event_create(pevent, NULL);
		ps_servant_cooperate();
	}

}

void actuator2()
{
	ps_event_t * pevent;
	ps_event_t * data;
	ps_event_t * tag;

	while(1){
		ps_event_wait();
		pevent = ps_event_receive();

		// process pevent including tag and data
		port_print("actuator 2\n\r");
        // output the event's data to physical device

		ps_servant_cooperate();
	}
}




