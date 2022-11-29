#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSystemTrayIcon>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void getTransactionCommand_doge(std::string transaction_str);
    void getTransactionCommand_bit(std::string transaction_str);
    
private slots:
    void on_actionExit_triggered();
    void on_tosendbrt_clicked();

    void showMessage();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
        void getTransactionFinish(QNetworkReply *rep);
    

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
