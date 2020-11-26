#pragma once


enum
{
	MQTT_THIRD = 0,		//������MQTT��Ϣ
	MQTT_LOCAL,			//RCU����MQTT��Ϣ
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
	QString topic;		// ����/��������
	QString payload;	// ����
	QString userName;
	QString password;
	QString clientId;
	QString hotelId;
	int ipAddress;		// �Ƿ�Ϊ����
	int wss;			// �Ƿ�Ϊwebsocket
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