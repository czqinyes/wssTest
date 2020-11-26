#pragma once


enum
{
	MQTT_THIRD = 0,		//第三方MQTT消息
	MQTT_LOCAL,			//RCU本地MQTT消息
};


struct MQTTStruct
{
	QString ip;
	int port;
	QString subscribe;
	QString deviceListSub;
	QString publish;
	QString deviceListPub;
	QString msg;
	QString topic;		// 订阅/发布主题
	QString payload;	// 内容
	QString userName;
	QString password;
	QString clientId;
	QString hotelId;
	int ipAddress;		// 是否为域名
	int wss;			// 是否为websocket
	MQTTStruct()
	{
		ip = "";
		port = 0;
		subscribe = "";
		publish = "";
		deviceListPub = "";
		msg = "";
		topic = "";
		payload = "";
		clientId = "";
		hotelId = "";
		ipAddress = 1;
		wss = 0;
	}
};