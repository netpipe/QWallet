#include "loadtheme.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

//LoadTheme::LoadTheme(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::LoadTheme)
//{
//    ui->setupUi(this);
//}

//LoadTheme::~LoadTheme()
//{
//    delete ui;
//}

//void LoadTheme::on_btnOpen_clicked()
//{
//    QString newFile = QFileDialog ::getOpenFileName(0,"Select File","","Files (*.qss)");
//    ui->editPath->setText(newFile);
//}

//void LoadTheme::on_btnOk_clicked()
//{
//    QString themeName = ui->editName->text();
//    QString themePath = ui->editPath->text();
//    if(themeName.isEmpty() || themePath.isEmpty()) {
//        QMessageBox::information(NULL, "Notice", "Set theme path and name correctly.");
//        return;
//    }

//    MainWindow::hInst->loadThemeFile(themePath, themeName);
//}
