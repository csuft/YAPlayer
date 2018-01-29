#include "insta360studio.h"
#include "../controls/msgbox.h"

#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include <QFile>

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(shaders);
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	app.setApplicationName("Insta360Player");
	app.setOrganizationName("Insta360");
	app.setOrganizationDomain("insta360.com");

	int retVal = 0;
	QFile file(":/style/global");
	file.open(QFile::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	// 使用共享内存来保证程序单实例运行
	QSharedMemory sharedMemory("Insta360Player");
	if (sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists) {
		player::Insta360Studio studio;
		studio.show();
		retVal = app.exec();
	}
	else {
		player::CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("警告：本程序实例已经运行！"), QPixmap(":/pics/warning"));
		msgBox.exec();
	}

	return retVal;
}
