#include "mymqtt.h"

#include <QHostAddress>
#include <QDateTime>
#include <QWebSocket>
#include <QSslSocket>

MyMQTT::MyMQTT(QObject *parent)
	: QObject(parent), m_mqttClient(NULL)
{
	// 无需定时器检测，已经设置每5秒自动重连
//	connect(&m_timer, &QTimer::timeout, this, &MyMQTT::slot_reconnect);
//	m_timer.start(5000);
}

MyMQTT::~MyMQTT()
{
	if (m_mqttClient)
	{
		closeMQTT();
		m_mqttClient->deleteLater();
	}
}


void MyMQTT::openMQTT(MQTTStruct mqtt)
{
	if (m_mqttClient == NULL)
	{
		qRegisterMetaType<QMQTT::ClientError>("QMQTT::ClientError");
		qRegisterMetaType<QMQTT::Message>("QMQTT::Message");
		qRegisterMetaType<QList<QSslError>>("const QList<QSslError>");

		if (!mqtt.wss)
		{
			m_mqttClient = new QMQTT::Client;
		}
		else
		{
			m_mqttClient = new QMQTT::Client(mqtt.ip, "<origin>", QWebSocketProtocol::VersionLatest);
		}

		connect(m_mqttClient, &QMQTT::Client::connected, this, &MyMQTT::slot_conncted);
		connect(m_mqttClient, &QMQTT::Client::disconnected, this, &MyMQTT::slot_disconnected);
		connect(m_mqttClient, &QMQTT::Client::error, this, &MyMQTT::slot_error);
		connect(m_mqttClient, &QMQTT::Client::sslErrors, this, &MyMQTT::slot_sslErrors);
		connect(m_mqttClient, &QMQTT::Client::subscribed, this, &MyMQTT::slot_subscribed);
		connect(m_mqttClient, &QMQTT::Client::unsubscribed, this, &MyMQTT::slot_unsubscribed);
		connect(m_mqttClient, &QMQTT::Client::received, this, &MyMQTT::signal_receivedMQTTMsg);
	}
	if (m_mqttClient)
	{
		if (m_mqttClient->isConnectedToHost())
		{
			m_mqttClient->disconnectFromHost();
		}
		m_mqttMsg = mqtt;	
		 
		QHostAddress host(mqtt.ip); 
		
		if (!mqtt.clientId.isEmpty())
			m_mqttClient->setClientId(mqtt.clientId);
		else
			m_mqttClient->setClientId(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
		if (!mqtt.wss)
		{
			m_mqttClient->setHost(host);
			//m_mqttClient->setPort(mqtt.port);
		}
		m_mqttClient->setPort(mqtt.port);
		if (!mqtt.userName.isEmpty() || !mqtt.password.isEmpty())
		{
			m_mqttClient->setUsername(mqtt.userName);
			m_mqttClient->setPassword(mqtt.password.toLocal8Bit());
		}
		m_mqttClient->setVersion(QMQTT::MQTTVersion::V3_1_1);
		//m_mqttClient->setKeepAlive(10);
		m_mqttClient->setAutoReconnect(true);
		m_mqttClient->setAutoReconnectInterval(1000);
		m_mqttClient->setCleanSession(true);
		m_mqttClient->connectToHost();
	}
}

void MyMQTT::closeMQTT()
{
	if (m_mqttClient)
	{
		if (m_mqttClient->isConnectedToHost())
		{
			m_mqttClient->disconnectFromHost();
		}
	}
}

void MyMQTT::subscribe(const QString &topic, const quint8 qos)
{
	qDebug() << topic;
	if (m_mqttClient)
	{
		if (m_mqttClient->isConnectedToHost())
		{
			m_mqttClient->subscribe(topic, qos);
		}
	}
}

void MyMQTT::unsubscribe(const QString &topic)
{
	if (m_mqttClient)
	{
		if (m_mqttClient->isConnectedToHost())
		{
			m_mqttClient->unsubscribe(topic);
		}
	}
}

quint16 MyMQTT::publish(const QString &topic, const QString &payload)
{
	if (m_mqttClient)
	{
		if (m_mqttClient->isConnectedToHost())
		{
			QMQTT::Message msg(0, topic, payload.toUtf8());
			return m_mqttClient->publish(msg);
		}
	}
	return 0;
}

bool MyMQTT::getConnectStatus()
{
	return m_bConnected;
}

void MyMQTT::slot_openMQTT(MQTTStruct mqtt)
{
	openMQTT(mqtt);
}

void MyMQTT::slot_closeMQTT()
{
	closeMQTT();
}

void MyMQTT::slot_publish(const QString &topic, const QString &payload)
{
	publish(topic, payload);
}

void MyMQTT::slot_mqttState()
{
	qDebug() << "connectionState: " << m_mqttClient->connectionState()<< "  clientId: " << m_mqttClient->clientId();
}

void MyMQTT::slot_conncted()
{
	qDebug() << "slot_conncted";
	m_bConnected = true;
	//QStringList subscribes = m_mqttMsg.subscribe.split(",");
	//for (int i = 0; i < subscribes.size(); i++)
	//{
	//	subscribe(QString(subscribes[i]).arg(m_mqttMsg.hotelId));
	//}
	//if (!m_mqttMsg.deviceListSub.isEmpty())
	//{
	//	subscribe(QString(m_mqttMsg.deviceListSub).arg(m_mqttMsg.hotelId));
	//}
	emit signal_mqttConnectStatus(true);
}

void MyMQTT::slot_disconnected()
{
	qDebug() << "slot_disconnected";
	if (m_bConnected)
	{
		emit signal_mqttConnectStatus(false);
	}
	m_bConnected = false;

}

void MyMQTT::slot_error(const QMQTT::ClientError error)
{
	qDebug() << "QMQTT::ClientError" << error;
	if (m_bConnected)
	{
		emit signal_mqttConnectStatus(false);
	}
	m_bConnected = false;
}

void MyMQTT::slot_subscribed(const QString& topic, const quint8 qos)
{
	emit signal_mqttSubscribeStatus(true);
}

void MyMQTT::slot_unsubscribed(const QString& topic)
{
	emit signal_mqttSubscribeStatus(false);
}

void MyMQTT::slot_reconnect()
{
	if (m_mqttClient == NULL) return;

	if (!m_mqttMsg.ip.isEmpty() && !m_mqttClient->isConnectedToHost())
	{
		QHostAddress host(m_mqttMsg.ip);
		m_mqttClient->setClientId(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
		m_mqttClient->setHost(host);
		m_mqttClient->setPort(m_mqttMsg.port);
		m_mqttClient->connectToHost();
	}
}

void MyMQTT::slot_sslErrors(const QList<QSslError> &errors)
{
	for (int i = 0;i < errors.size();++i)
	{
		qDebug() << errors[i].errorString();
	}
}

