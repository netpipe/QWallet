#include <QtGui>
#include <QApplication>
#include "qstylesheetmanager.h"

QStyleSheetManager::QStyleSheetManager(void)
{
}

QStyleSheetManager::~QStyleSheetManager(void)
{
}

void QStyleSheetManager::loadStyleSheet(const QString& sheet_name)
{
    QFile file(sheet_name);    // Modified by RCU 2016.07.17
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String( file.readAll() );

    qApp->setStyleSheet(styleSheet);
}

void QStyleSheetManager::loadStyleSheet(const QString& sheet_name, QWidget* widget)
{
    QFile file("/" + sheet_name + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    widget->setStyleSheet(styleSheet);
}

//void QStyleSheetManager::modifyStyle(QString Gradient){

//}
