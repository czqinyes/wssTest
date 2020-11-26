#pragma once

#include <QObject>

#include <QTimer>

#include "DefineStruct.h"
#include "mqtt/qmqtt.h"

class MyMQTT : public QObject
{
	Q_OBJECT

public:
	MyMQTT(QObject *parent = 0);
	~MyMQTT();

public:
	void openMQTT(MQTTStruct mqtt);
	void closeMQTT();
	void subscribe(const QString &topic, const quint8 qos = 0);
	void unsubscribe(const QString &topic);
	quint16 publish(const QString &topic, const QString &payload);

	bool getConnectStatus();

public slots:
	void slot_openMQTT(MQTTStruct mqtt);
	void slot_closeMQTT();
	void slot_publish(const QString &topic, const QString &payload);

	void slot_mqttState();

private slots:
	void slot_conncted();
	void slot_disconnected();
	void slot_error(const QMQTT::ClientError error);
	void slot_subscribed(const QString& topic, const quint8 qos);
	void slot_unsubscribed(const QString& topic);

	void slot_reconnect();

	void slot_sslErrors(const QList<QSslError> &errors);

signals:
	void signal_mqttConnectStatus(bool bConnected);
	void signal_mqttSubscribeStatus(bool bSubscribed);
	void signal_receivedMQTTMsg(QMQTT::Message message);

private:
	QMQTT::Client *m_mqttClient;

	QTimer m_timer;

	MQTTStruct m_mqttMsg;

	bool m_bConnected;
};
