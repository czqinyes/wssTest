#include "wsstest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	wssTest w;
	w.show();
	return a.exec();
}
