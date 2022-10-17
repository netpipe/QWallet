#ifndef LOADTHEME_H
#define LOADTHEME_H

//#include <QWidget>
#include <mainwindow.h>
#include "ui_mainwindow.h"
//#include <QCoreApplication>
#include "src/qstylesheetmanager.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QDirIterator>

//void MainWindow::themeInit(){



//    QFile MyFile("themes.txt");
//    MyFile.open(QIODevice::ReadWrite);
//    QTextStream in (&MyFile);
//    QString line;
//    QStringList list;
//    //   QList<QString> nums;
//    QStringList nums;
//    QRegExp rx("[:]");
//    line = in.readLine();

//    if (line.contains(":")) {
//        list = line.split(rx);
//        qDebug() << "theme" <<  list.at(1).toLatin1();
//        QStyleSheetManager::loadStyleSheet( list.at(1).toLatin1());
//    }

//    QDirIterator it("./Resource/themes/", QStringList() << "*.qss", QDir::Files, QDirIterator::Subdirectories);
//    while (it.hasNext()){
//        //  QFileInfo fileInfo(f.fileName());
//        ui->cmbTheme->addItem(it.next().toLatin1());
//    }
//    loaded=true;
//    //    do {

//    //    } while (!line.isNull());

//    //    ui->cmbTheme->itemText(ui->cmbTheme->count());

//    //    if (ui->cmbTheme->currentText().toLatin1() != ""){
//    //        //QStyleSheetManager::loadStyleSheet( ui->cmbTheme->currentText().toLatin1());
//    //        QStyleSheetManager::loadStyleSheet(  ui->cmbTheme->itemText(ui->cmbTheme->count()-1));
//    //    } else {}

//}



//void MainWindow::on_cmbTheme_currentIndexChanged(const QString &arg1)
//{
//    if (loaded==true)
//    {
//    fileName=ui->cmbTheme->currentText();
//    QFile file(fileName);

//    QStyleSheetManager::loadStyleSheet(ui->cmbTheme->currentText());

//    QFile file2("themes.txt");
//        if(file2.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
//        {
//                QTextStream stream(&file2);
//                file2.seek(0);
//               stream << "theme:" << ui->cmbTheme->currentText().toLatin1()<< endl;
//                for (int i = 0; i < ui->cmbTheme->count(); i++)
//                {
//                 stream << "theme:" << ui->cmbTheme->itemText(i) << endl;
//                }
//            //                file.write("\n");
//               file2.close();
//        }

//    if (ui->cmbTheme->currentText().toLatin1() != ""){
//      //   ui->cmbTheme->currentText().toLatin1();
//    }
//}

//}


//void MainWindow::on_btnAddThemeFromFile_clicked()
//{
//    QString newFile = QFileDialog ::getOpenFileName(0,"Select File","","Files (*.qss)");
//    ui->cmbTheme->addItem(newFile);

//    QFile file("themes.txt");
//        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
//        {
//                QTextStream stream(&file);
//                file.seek(0);
//                for (int i = 0; i < ui->cmbTheme->count(); i++)
//                {
//                 stream << "theme:" << ui->cmbTheme->itemText(i) << endl;
//                }
//            //                file.write("\n");
//               file.close();
//        }
//}

//void MainWindow::on_btnRemoveThemeFromFile_clicked()
//{
//    ui->cmbTheme->removeItem(ui->cmbTheme->currentIndex());
//    QFile file("themes.txt");
//    if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
//    {
//        QTextStream stream(&file);
//        file.seek(0);
//        for (int i = 0; i < ui->cmbTheme->count(); i++)
//        {
//            stream << ui->cmbTheme->itemText(i) << endl;
//        }
//     //file.write("\n");
//     file.close();
//     }
//}

//void MainWindow::on_scantheme_clicked()
//{
//    ui->cmbTheme->clear();
//    QDirIterator it("./Resource/themes/", QStringList() << "*.qss", QDir::Files, QDirIterator::Subdirectories);
//    while (it.hasNext()){
//      //  QFileInfo fileInfo(f.fileName());
//        ui->cmbTheme->addItem(it.next().toLatin1());
//    }
//    on_btnApply_clicked(); //save files

//}

//void MainWindow::on_btnApply_clicked() //theme
//{
//    QFile file("themes.txt");
//        if(file.open(QIODevice::ReadWrite | QIODevice::Text))// QIODevice::Append |
//        {
//                QTextStream stream(&file);
//                file.seek(0);
//               stream << "theme:" << ui->cmbTheme->currentText().toLatin1()<< endl;
//                for (int i = 0; i < ui->cmbTheme->count(); i++)
//                {
//                 stream << "theme:" << ui->cmbTheme->itemText(i) << endl;
//                }
//            //                file.write("\n");
//               file.close();
//        }

//    if (ui->cmbTheme->currentText().toLatin1() != ""){
//        QStyleSheetManager::loadStyleSheet( ui->cmbTheme->currentText().toLatin1());
//    }
//}


#endif // LOADTHEME_H
