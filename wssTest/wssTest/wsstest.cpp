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

	m_pProcLocalMQTT = new ProcLocalMQTT(0);
	connect(m_pProcLocalMQTT, &ProcLocalMQTT::signal_mqttConnectStatus, this, &wssTest::slot_mqttConnectStatus);
	connect(m_pProcLocalMQTT, &ProcLocalMQTT::signal_sendMQTTMsg, this, &wssTest::slot_recvMQTTMsg);

	MQTTStruct localmqtt;
	//////////////////////////////// test wss /////////////////////////////
	/*localmqtt.ip = "wss://xxxxx/mqtt";
	localmqtt.port = ;
	localmqtt.clientId = "";
	localmqtt.userName = "";
	localmqtt.password = "";
	localmqtt.wss = 1;*/

	//////////////////////////////// test tcp /////////////////////////////
	localmqtt.ip = "127.0.0.1";
	localmqtt.port = 1883;
	localmqtt.wss = 0;

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
	ui.textBrowser->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz   ") + QString("%1").arg(bConnected == true ? tr("success") : tr("fail")));
}



void wssTest::slot_recvMQTTMsg(const int type, const QString &topic, const QString &payload)
{ 
	
	ui.textBrowser->append(QString("topic: %1;   payload: %2;").arg(topic).arg(payload));
}