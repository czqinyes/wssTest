#include "wsstest.h"

#include "DefineStruct.h"

#pragma execution_character_set ("utf-8")

#include <QMessageBox>
#include <QDateTime>

wssTest::wssTest(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	if (!QSslSocket::supportsSsl()) {
		QMessageBox::information(0, "Secure Socket Client",
			"This system does not support OpenSSL.");
	}

	QSslSocket *sslSocket = new QSslSocket(this);
	sslSocket->setProtocol(QSsl::TlsV1_2);
	QString aa = QSslSocket::sslLibraryVersionString();
	qDebug()<< sslSocket->protocol()<< QSslSocket::sslLibraryVersionString();

	//QMessageBox::information(this, "tip", QString("protocol:%1; sslLibraryVersionString:%2;").
	//	arg(sslSocket->protocol()).arg(QSslSocket::sslLibraryVersionString()), QMessageBox::Ok);

//	qDebug() << QSslSocket::sslLibraryVersionString();

	m_pProcLocalMQTT = new ProcLocalMQTT(0);
	connect(m_pProcLocalMQTT, &ProcLocalMQTT::signal_mqttConnectStatus, this, &wssTest::slot_mqttConnectStatus);
	connect(m_pProcLocalMQTT, &ProcLocalMQTT::signal_sendMQTTMsg, this, &wssTest::slot_recvMQTTMsg);

	MQTTStruct localmqtt;

	localmqtt.ip = "broker.emqx.io";
	localmqtt.port = 8084;
	//localmqtt.clientId = "008301015fa11a11e4b0a3733ec344c6_0_0_2020110413";
	//localmqtt.userName = "008301015fa11a11e4b0a3733ec344c6";
	//localmqtt.password = "372ba8ed034013aa2476fda1e148af0d65b3e952c2e002cdfd09286e58089def";
	localmqtt.wss = 1;

	/*localmqtt.ip = "wss://apigw-beta.huawei.com:443/ws/IoTAgent/Wss?X-HW-ID=com.huawei.pangu.iot.hotel&X-HW-APPKEY=LUu2%2BYTkldBdlB9wYyCW7w%3D%3D";
	localmqtt.port = 443;
	localmqtt.clientId = "008301015fa11a11e4b0a3733ec344c6_0_0_2020110413";
	localmqtt.userName = "008301015fa11a11e4b0a3733ec344c6";
	localmqtt.password = "372ba8ed034013aa2476fda1e148af0d65b3e952c2e002cdfd09286e58089def";
	localmqtt.wss = 1;*/

	/*localmqtt.ip = "wss://spasrelax.com.cn/mqtt";
	localmqtt.port = 1883;
	localmqtt.clientId = "008301015fa11a11e4b0a3733ec344c6_0_0_2020110413";
	localmqtt.userName = "303758083@qq.com";
	localmqtt.password = "e35cf7b66449df565f93c607d5a81d09";
	localmqtt.wss = 1;*/

	/*localmqtt.ip = "127.0.0.1";
	localmqtt.port = 1883;
	localmqtt.clientId = "008301015fa11a11e4b0a3733ec344c6_0_0_2020110413";
	localmqtt.userName = "303758083@qq.com";
	localmqtt.password = "e35cf7b66449df565f93c607d5a81d09";
	localmqtt.wss = 0;*/

	//localmqtt.subscribe = encryptData(setting.value("LocalMQTTInfo/subscribe", "").toString().toLocal8Bit());
	//localmqtt.publish = QString(encryptData(setting.value("LocalMQTTInfo/publish", "").toString().toLocal8Bit())).arg(localmqtt.hotelId);
	if (m_pProcLocalMQTT)
	{
		m_pProcLocalMQTT->openMQTT(localmqtt);
	}
}

wssTest::~wssTest()
{

}

void wssTest::slot_mqttConnectStatus(const int type, bool bConnected)
{
	ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz   ") + QString("%1").arg(bConnected == true ? tr("连接成功") : tr("连接失败")));
}



void wssTest::slot_recvMQTTMsg(const int type, const QString &topic, const QString &payload)
{ 
	
	ui.textBrowser->append(QString("topic: %1;   payload: %2;").arg(topic).arg(payload));
}