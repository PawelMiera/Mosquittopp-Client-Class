#include "MQTT.h"


MQTT::MQTT(const char * _host, int _port)
{
	mosqpp::lib_init();        
	keepalive = 60;    
	port = _port;
	host = _host;
	connect_async(host, port,keepalive);
	loop_start();
	std::cout<<"MQTT On\n";
}

MQTT::MQTT(const char * _host, int _port,int _keepalive)
{
	mosqpp::lib_init();    
	keepalive = _keepalive;  
	port = _port;
	host = _host;
	connect_async(host, port,keepalive);
	loop_start();            
	std::cout<<"MQTT On\n";
}

///////////////////////////////////| automatic constructor | /////////////////////////////////////
MQTT::MQTT()
{
	automatic=true;
	mosqpp::lib_init();        
	keepalive = auto_keepalive;   
	port = auto_port;
	host = auto_host;
	connect_async(host, port,keepalive);
	loop_start();        
	std::cout<<"MQTT On\n";	

}

MQTT::~MQTT() 
{
	loop_stop(true);        
	mosqpp::lib_cleanup();   
	std::cout<<"MQTT Off\n";
}

void MQTT::stop()
{
loop_stop(true);          
mosqpp::lib_cleanup();   
std::cout<<"MQTT Off\n";
}

void MQTT::mqtt_send_message(std::string topic, std::string msg)
{
	int ret = publish(nullptr, topic.c_str(), msg.size(), msg.c_str(), 1, false);
	if(ret == MOSQ_ERR_SUCCESS) std::cout<<"Message: '"<<msg<<"'"<<" send to topic: "<<topic<<std::endl;
	else std::cout<<"Message error\n";
}


void MQTT::mqtt_subscribe(std::string _topic)
{
	int ret = subscribe(nullptr, _topic.c_str());

	if(ret == MOSQ_ERR_SUCCESS) 
	{
		std::cout<<"Subscribed to topic: "<<_topic<<std::endl;
		topics.push_back(_topic);
	}
	else std::cout<<"Subscribtion error\n";
}

void MQTT::on_connect(int rc) 
{
	std::cout<<"Connected\n";
	if(reconnected)
	{
		reconnected=false;
		resubscribe();
	}
}

void MQTT::on_disconnect(int rc)
{
	std::cout<<"Disconnected\n";
	reconnected=true;
	reconnect_async();
}

void MQTT::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	std::cout << "On_subscribe\n";
}

void MQTT::on_error()
{
	std::cout << "On Error!" << std::endl;
}

void MQTT::on_message(const struct mosquitto_message *message)
{
	std::string msg_topic = message->topic;
	std::string msg_payload = std::string((char *)message->payload, message->payloadlen);

	std::cout <<"Received message: " << msg_payload << " on topic: " << msg_topic<< std::endl;

}

void MQTT::resubscribe()
{
	for(int i = 0; i<topics.size();i++)
	{
		int ret = subscribe(nullptr, topics[i].c_str());
	
		if(ret == MOSQ_ERR_SUCCESS) 
		{
			std::cout<<"Subscribed to topic: "<<topics[i]<<std::endl;
		}
		else std::cout<<"Subscribtion error\n";
	}
}


