#include <iostream>
#include "MQTT.h"



int main(int argc, char **argv)
{

	const char* host = "localhost";
	int port = 420;

	//MQTT mqtt_client;		//first constructor

	MQTT mqtt_client(host, port);		//second constructor

	//MQTT mqtt_client(host, port, keepalive);		//first constructor
	mqtt_client.mqtt_subscribe("topic/sample");
	mqtt_client.mqtt_send_message("topic/second","acd");
	char a;

	while (a != 'q')
	{
		a=std::tolower(std::cin.get());
	}

	mqtt_client.stop();	
    return 0;
}
