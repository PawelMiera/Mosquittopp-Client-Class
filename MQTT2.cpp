#include <iostream>
#include "mosquittopp.h"
#include <vector>
#include "MQTTClient.h"

class MQTTClient::myMosq : public virtual mosqpp::mosquittopp
{
private:
	const char *host;
	const char *id;
	std::vector<std::string> topics;
	int port, keepalive;

	bool automatic=false;
	const char *auto_host = "localhost";
	const char *auto_id = "ArduPilot";
	std::vector<std::string> auto_topics={"main/getTelemetry", "watchdog/telemtry"};
	int auto_port=420;
	int auto_keepalive=60;

	bool reconnected=false;

	 void on_connect(int rc) override
	{
		std::cout<<"connected\n";
		if(reconnected)
		{
			reconnected=false;
			resubscribe();
		}
	}

	void on_disconnect(int rc) override
	{
		std::cout<<"disconnected\n";
		reconnected=true;
		reconnect_async();
	}

	void on_publish(int mid) override
	{
	}

	void on_subscribe(int mid, int qos_count, const int *granted_qos) override
	{
		std::cout << "On_subscribe.\n";
	}
	void on_error() override
	{
		std::cout << "On Error!" << std::endl;
	}

	void on_message(const struct mosquitto_message *message) override
	{
		std::string msg_topic = message->topic;
		std::string msg_payload = std::string((char *)message->payload, message->payloadlen);

		std::cout <<"Received message: " << msg_payload << " on topic: " << msg_topic<< std::endl;

                if(msg_topic == "main/getTelemetry")
                {
                        if(msg_payload=="1")
                        {

				/*mavlink_local_position_ned_t pos = api.current_messages.local_position_ned;
				std::string x = std::to_string(pos.x);
				std::string y = std::to_string(pos.y);
				std::string z = std::to_string(pos.z);
				std::string output = "x"+x+"y"+y+"z"+z+"e";
				std::cout<<"replying: "<<output<<std::endl;
                                mqtt::topic top(cli_, PUBLISH_TOPIC, 1, true);
                               	top.publish(std::move(output));
				*/
				mqtt_send_message("main/telemetry", "telemetria");
                        }
                }
                if(msg_topic == "watchdog/telemetry")
                {
                        if(msg_payload=="1")
                        {

				/*mavlink_local_position_ned_t pos = api.current_messages.local_position_ned;
				std::string x = std::to_string(pos.x);
				std::string y = std::to_string(pos.y);
				std::string z = std::to_string(pos.z);
				std::string output = "x"+x+"y"+y+"z"+z+"e";
				std::cout<<"replying: "<<output<<std::endl;
                                mqtt::topic top(cli_, PUBLISH_TOPIC, 1, true);
                               	top.publish(std::move(output));
				*/
				mqtt_send_message("watchdogReply/telemetry", "1");
                        }
                }

	}

	void resubscribe()
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



public:
///////////////////////////////////| basic constructors | /////////////////////////////////////
	myMosq(const char * _id, const char * _host, int _port)
	{
		mosqpp::lib_init();        
		this->keepalive = 60;    
		this->id = _id;
		this->port = _port;
		this->host = _host;
		connect_async(host, port,keepalive);
		loop_start();
		std::cout<<"MQTT On\n";
	}
	myMosq(const char * _id, const char * _host, int _port,int _keepalive)
	{
		mosqpp::lib_init();    
		this->keepalive = _keepalive;  
		this->id = _id;
		this->port = _port;
		this->host = _host;
		connect_async(host, port,keepalive);
		loop_start();            
		std::cout<<"MQTT On\n";
	}
///////////////////////////////////| automatic constructor | /////////////////////////////////////
	myMosq()
	{
		automatic=true;
		mosqpp::lib_init();        
		keepalive = auto_keepalive;   
		id = auto_id;
		port = auto_port;
		host = auto_host;
		connect_async(host, port,keepalive);
		loop_start();        
		std::cout<<"MQTT On\n";
		for(int i = 0; i<auto_topics.size();i++)
		{
			mqtt_subscribe(auto_topics[i]);
		}
		

	}

	~myMosq() 
	{
		loop_stop(true);        
		mosqpp::lib_cleanup();   
		std::cout<<"MQTT Off\n";
	}
	
	void stop()
	{
	loop_stop(true);          
	mosqpp::lib_cleanup();   
	std::cout<<"MQTT Off\n";
	}

	void mqtt_subscribe(std::string _topic)
	{
		int ret = subscribe(nullptr, _topic.c_str());

		if(ret == MOSQ_ERR_SUCCESS) 
		{
			std::cout<<"Subscribed to topic: "<<_topic<<std::endl;
			topics.push_back(_topic);
		}
		else std::cout<<"Subscribtion error\n";
	}	

	void mqtt_send_message(std::string topic, std::string msg)
	{
		int ret = publish(nullptr, topic.c_str(), msg.size(), msg.c_str(), 1, false);
		if(ret == MOSQ_ERR_SUCCESS) std::cout<<"Message: '"<<msg<<"'"<<" send to topic: "<<topic<<std::endl;
		else std::cout<<"Message error\n";
	}

};
