#include <iostream>
#include "MQTT.h"



int main(int argc, char **argv)
{

	MQTT mqtt_client; //(autopilot_interface)

	char a;
	while (a != 'q')
	{
		a=std::tolower(std::cin.get());
	}


	mqtt_client.stop();	
    return 0;
}
