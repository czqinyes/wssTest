#pragma once

#include <QObject>

#include "DefineStruct.h"
#include "mymqtt.h"

class ProcLocalMQTT : public QObject
{
	Q_OBJECT

public:
	explicit ProcLocalMQTT(QObject *parent);
	~ProcLocalMQTT();

	void openMQTT(MQTTStruct mqtt);
	void closeMQTT();
	void subscribe(const QString &topic, const quint8 qos = 0);
	void unsubscribe(const QString &topic);
	quint16 publish(const QString &topic, const QString &payload);

private slots:
	void slot_mqttConnectStatus(bool bConnected);
	void slot_receivedMQTTMsg(QMQTT::Message message);

signals:
	void signal_openMQTT(MQTTStruct mqtt);
	void signal_closeMQTT();
	void signal_publish(const QString &topic, const QString &payload);

	void signal_mqttConnectStatus(const int type, bool bConnected);
	void signal_mqttSubscribeStatus(bool bSubscribed);
	//void signal_showMQTTMsg(MQTTStruct mqtt);

	void signal_sendMQTTMsg(const int type, const QString &topic, const QString &payload);

private:
	MyMQTT *m_pMyMQTT;
};
