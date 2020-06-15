#ifndef MQTT_H
#define MQTT_H 

#include <iostream>
#include "mosquittopp.h"
#include <vector>
#include <unistd.h>

class MQTT : public mosqpp::mosquittopp
{
public:

	MQTT(const char * _host, int _port);
	MQTT(const char * _host, int _port,int _keepalive);
	MQTT();
	~MQTT();
	void stop();
	void mqtt_send_message(std::string topic, std::string msg);
	void mqtt_subscribe(std::string _topic);

private:

	const char *host;
	std::vector<std::string> topics;
	int port, keepalive;
	bool reconnected=true;

	bool automatic=false;
	const char *auto_host = "localhost";
	std::vector<std::string> auto_topics={"main/getTelemetry", "watchdog/telemtry"};
	int auto_port=420;
	int auto_keepalive=60;

	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_subscribe(int mid, int qos_count, const int *granted_qos);
	void on_error();
	void on_message(const struct mosquitto_message *message);
	void resubscribe();

};


#endif
