#include "mainwindow.h"
#include <QtGui/QApplication>
#include <windows.h>

#include "sfacealignment.h"
#include "sface.h"
#include "sface_dict.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w(0);
	w.show();
	return a.exec();
}
