#include "proclocalmqtt.h"

#include <QThread>

ProcLocalMQTT::ProcLocalMQTT(QObject *parent)
	: QObject(parent)
{
	
	m_pMyMQTT = new MyMQTT;
	QTimer* timer = new QTimer;
	connect(timer, &QTimer::timeout, m_pMyMQTT, &MyMQTT::slot_mqttState);
	//timer->start(1000);
	connect(m_pMyMQTT, &MyMQTT::signal_mqttConnectStatus, this, &ProcLocalMQTT::slot_mqttConnectStatus);
	connect(m_pMyMQTT, &MyMQTT::signal_mqttSubscribeStatus, this, &ProcLocalMQTT::signal_mqttSubscribeStatus);
	connect(m_pMyMQTT, &MyMQTT::signal_receivedMQTTMsg, this, &ProcLocalMQTT::slot_receivedMQTTMsg);
	qRegisterMetaType<MQTTStruct>("MQTTStruct");
	connect(this, &ProcLocalMQTT::signal_openMQTT, m_pMyMQTT, &MyMQTT::slot_openMQTT);
	connect(this, &ProcLocalMQTT::signal_closeMQTT, m_pMyMQTT, &MyMQTT::slot_closeMQTT);
	connect(this, &ProcLocalMQTT::signal_publish, m_pMyMQTT, &MyMQTT::slot_publish);

	QThread *thread = new QThread;
	connect(thread, &QThread::finished, this, &QThread::deleteLater);
	m_pMyMQTT->moveToThread(thread);
	thread->start();
}

ProcLocalMQTT::~ProcLocalMQTT()
{
	if (m_pMyMQTT)
	{
		m_pMyMQTT->closeMQTT();
		m_pMyMQTT->deleteLater();
	}
}

void ProcLocalMQTT::openMQTT(MQTTStruct mqtt)
{
	if (m_pMyMQTT)
	{
		emit signal_openMQTT(mqtt);
	}
}

void ProcLocalMQTT::closeMQTT()
{
	if (m_pMyMQTT)
	{
		emit signal_closeMQTT();
	}
}

void ProcLocalMQTT::subscribe(const QString &topic, const quint8 qos)
{
	if (m_pMyMQTT)
	{
		m_pMyMQTT->subscribe(topic, qos);
	}
}

void ProcLocalMQTT::unsubscribe(const QString &topic)
{
	if (m_pMyMQTT)
	{
		m_pMyMQTT->unsubscribe(topic);
	}
}

quint16 ProcLocalMQTT::publish(const QString &topic, const QString &payload)
{
	if (m_pMyMQTT)
	{
		emit signal_publish(topic, payload);
	}
	return 0;
}

void ProcLocalMQTT::slot_mqttConnectStatus(bool bConnected)
{
	emit signal_mqttConnectStatus(MQTT_LOCAL, bConnected);
}

void ProcLocalMQTT::slot_receivedMQTTMsg(QMQTT::Message message)
{
	emit signal_sendMQTTMsg(MQTT_LOCAL, message.topic(), message.payload());
}


