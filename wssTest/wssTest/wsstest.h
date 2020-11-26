#ifndef WSSTEST_H
#define WSSTEST_H

#include <QtWidgets/QMainWindow>
#include "ui_wsstest.h"

#include "proclocalmqtt.h"

class wssTest : public QMainWindow
{
	Q_OBJECT

public:
	wssTest(QWidget *parent = 0);
	~wssTest();

private slots:
// mqtt连接状态
void slot_mqttConnectStatus(const int type, bool bConnected);
// mqtt消息
void slot_recvMQTTMsg(const int type, const QString &topic, const QString &payload);

private:
	ProcLocalMQTT *m_pProcLocalMQTT;

private:
	Ui::wssTestClass ui;
};

#endif // WSSTEST_H
