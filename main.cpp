#include "firstopencvwithqt.h"
#include <QtWidgets/QApplication>
#include "highgui.h"
#include "qlabel.h"
#include <QMessageBox>

int BtnTestwithOCV();

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FirstOpenCVwithQt w;
	w.show();

	return a.exec();
}

