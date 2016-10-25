void sensor1()
{
	ps_event_wait();
	port_print("sensor 1\n\r");
	ps_servant_cooperate();
}

void controller1()
{
	ps_event_wait();
	port_print("controller 1\n\r");
	ps_servant_cooperate();
}

void actuator1()
{
	ps_event_wait();
	port_print("actuator 1\n\r");
	ps_servant_cooperate();
}

void sensor2()
{
	ps_event_wait();
	port_print("sensor 2\n\r");
	ps_servant_cooperate();
}

void controller2()
{
	ps_event_wait();
	port_print("controller 2\n\r");
	ps_servant_cooperate();
}

void actuator2()
{
	ps_event_wait();
	port_print("actuator 2\n\r");
	ps_servant_cooperate();
}




