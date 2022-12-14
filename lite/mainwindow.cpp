#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "crypto/TestHelper.hpp"
#include "crypto/Ecdsa.hpp"
#include "crypto/Sha256Hash.hpp"
#include "crypto/Uint256.hpp"
#include "address.h"
#include "QJsonObject"
#include "qdebug.h"
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include "cpp_bitcoin_rpc/bitcoin.hpp"

#include <QMessageBox>

//https://www.blockchain.com/explorer/api/blockchain_api
//
//https://github.com/Neyoui/DogeChainAPI
//https://dogechain.info/api/blockchain_api
//https://www.blockchain.com/explorer/api/blockchain_api
//https://doc.qt.io/qt-5/qtcore-serialization-savegame-example.html

//https://www.weiy.city/2020/08/how-to-write-and-read-json-file-by-qt/
//https://dogechain.info/api/v1/pushtx?tx=hashvalue
//https://chain.so/api/#get-prices

//curl https://chain.so/api/v2/get_tx_unspent/DOGE/DRapidDiBYggT1zdrELnVhNDqyAHn89cRi/e83d147c3bcd87c6efd5270896a179f6ecb1de8b8c8cc324645c5a14129baf8c


static int numTestCases = 0;

static void testEcdsaSignAndVerify()
{
    // Define test cases
    struct SignCase
    {
        bool matches;
        const char *privateKey;
        const char *msgHash;  // Byte-reversed
        const char *nonce;    // Can be null
        const char *expectedR;
        const char *expectedS;
    };

    const vector<SignCase> cases
    {
        // Hand-crafted cases
        {true, "0000000000000000000000000000000000000000000000000000000000000123",
        "8900000000000000000000000000000000000000000000000000000000000000",
        "0000000000000000000000000000000000000000000000000000000000000457",
        "28B7F3A019749CCE6FC677AFA8FAE72EC10E811ED4B04E1963143CEF87654B75",
        "04719F34FE9A47F2C9A22045485F3654DC3AC4A910A7B0B4C7A318F41DB65C9B"}//,
//		{true, "8B46893E711C8948B28E7637BFBED61666E0118ED4D361BED1F18058214C69B8",
//		"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
//		"D9063703D9F719739FF645C77BA2F9D1DD2B4254DC7B001F8FC77C3B05AEF5B1",
//		"B4508AF745210F6702C687682FD5E8C8D99CD1C6A7AD450AB4640458E14474BA",
//		"421ED1256C6056D50A481D76B77CF5AA74A692556682E584A4872E8D8BBBCEAC"},
//		{true, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364140",
//		"B8EF4E4640FBBD526166FF260EB65EC2B3B60064CCE2DA9747201BA201E90F7F",
//		"E2EF043987542CD685482E71E57D01A93B701C2610FB03253871DC2958AD3D73",
//		"38C2AB97F778D0E1E468B3A7EBEBD2FB1C45678B62DD01587CF54E298C71EC43",
//		"7A6D055110F4296A85E8122B32F87AD32A58CF9BB73435088102638D7DECE1F2"}
    };

    // Test runner
    for (const SignCase &tc : cases)
    {
        Uint256 privateKey(tc.privateKey);
        const Sha256Hash msgHash(tc.msgHash);
        Uint256 expectedR(tc.expectedR);
        Uint256 expectedS(tc.expectedS);

        Uint256 r, s;
        bool ok;
        if (tc.nonce != nullptr)
        {
            Uint256 nonce(tc.nonce);
            ok = Ecdsa::sign(privateKey, msgHash, nonce, r, s);
        } else
        {
            ok = Ecdsa::signWithHmacNonce(privateKey, msgHash, r, s);
        }
        bool actualMatch = r == expectedR && s == expectedS;
        assert(ok && actualMatch == tc.matches);

        if (Uint256::ZERO < privateKey && privateKey < CurvePoint::ORDER)
        {
            CurvePoint publicKey = CurvePoint::privateExponentToPublicPoint(privateKey);
            assert(Ecdsa::verify(publicKey, msgHash, r, s));
        }

        numTestCases++;
    }
}

static void testEcdsaVerify()
{
    struct VerifyCase
    {
        bool answer;
        const char *pubPointX;
        const char *pubPointY;
        const char *msgHash;  // Byte-reversed
        const char *rValue;
        const char *sValue;
    };

    const vector<VerifyCase> cases
    {
        {false, "77D9ECB1D22A45C107EE36FC6D62A4D32BAB6689A50F0FAE587E0B95A795E833",
        "9BB5CF3051C7FCD5B69CB80A59B052D75BB6C6090B28C1E5AC0C6502B04BE63B",
        "EF54D03E7453CED1A0A9529ADFBE46CE7440E40E3457CA1C040B6CAC9E3209E4",
        "EB4E0C2C1723EFE8192F2F8743D343F45B5B8A9A12012EE71743247B0F65DAD8",
        "08F4E06799E5919F72EE39D3473EB473BD8ADC672694D895734E8AE4D049E038"},
    };

    for (const VerifyCase &tc : cases)
    {
        CurvePoint publicKey(tc.pubPointX, tc.pubPointY);
        const Sha256Hash msgHash(tc.msgHash);
        Uint256 r(tc.rValue);
        Uint256 s(tc.sValue);
        assert(Ecdsa::verify(publicKey, msgHash, r, s) == tc.answer);
        numTestCases++;
    }
}

QSystemTrayIcon *trayIcon;
QMenu *trayIconMenu;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap oPixmap(32,32);
    oPixmap.load ( "smoking.png");

    QIcon oIcon( oPixmap );

    trayIcon = new QSystemTrayIcon(oIcon);

    QAction *quit_action = new QAction( "Exit", trayIcon );
    connect( quit_action, SIGNAL(triggered()), this, SLOT(on_exit()) );

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction( quit_action );

    trayIcon->setContextMenu( trayIconMenu);
    //trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);
    trayIcon->setVisible(true);
    //trayIcon->show();

    generateAddress();

    testEcdsaSignAndVerify();
    testEcdsaVerify();
    std::printf("All %d test cases passed\n", numTestCases);


    //   QJsonObject jsonObj ;
    QJsonObject content;
    QJsonDocument document;
    document.setObject( content );
    QFile file( "test.test" );

    QString mName;

    if( file.open( QIODevice::ReadOnly ) )
    {
        QByteArray bytes = file.readAll();
        file.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson( bytes, &jsonError );
        if( jsonError.error != QJsonParseError::NoError )
        {
            qDebug() << "fromJson failed: " ;//<< jsonError.errorString().toStdString() << endl;
            return ;
        }
        if( document.isObject() )
        {
            QJsonObject jsonObj = document.object();
            if (jsonObj.contains("FirstName") && jsonObj["FirstName"].isString())
            mName = jsonObj["FirstName"].toString();
            qDebug() << mName.toLatin1() << "test";
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
     case QSystemTrayIcon::Trigger:
         this->show();
         break;
     case QSystemTrayIcon::DoubleClick:
         this->show();
         break;
     case QSystemTrayIcon::MiddleClick:
      //  showMessage();
        break;

     default:
         ;
    }
}
void MainWindow::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon();
    trayIcon->showMessage(tr("QSatisfy"), tr("Will you smoke now..."), icon, 100);
}

void MainWindow::on_tosendbrt_clicked()
{
    boost::asio::io_service ioservice;

    bitcoin::client m_client(ioservice);
    bool bconnect = m_client.connect("http://localhost:8545", "user", "pass");
    //if( !bconnect ) return;

    std::string transaction = m_client.createrawtransaction("[{\"txid\":\"e6da89de7a6b8508ce8f371a3d0535b04b5e108cb1a6e9284602d3bfd357c018\",\"vout\":1}]",
                                  "{\"13cgrTP7wgbZYWrY9BZ22BV6p82QXQT3nY\": 0.49213337}");

    bool bsign = m_client.signrawtransaction(transaction);

    if(bsign)
        printf("Transaction Sign Successful.\n");
    else
        printf("Transaction Sign Successful.\n");

    std::string res = m_client.sendrawtransaction(transaction);



//    printf("Send Transaction Result: %s\n", res);
//        if(ui->select_type->currentText() == "dogecoin")
//    {
        getTransactionCommand_doge("bcc4ce3f004b02b50456976dfc4de69d651ba718ce95422f51cd1d4bf7235abb");
//    }
//    else if(ui->select_type->currentText() == "bitcoin")
//    {
 //       getTransactionCommand_bit("bcc4ce3f004b02b50456976dfc4de69d651ba718ce95422f51cd1d4bf7235abb");
//    }
//    else
    
}

void MainWindow::getTransactionCommand_doge(std::string transaction_str)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)), this, SLOT(getTransactionFinish(QNetworkReply*)));

    QString url_path ="https://dogechain.info/api/v1/transaction/";
    url_path += QString::fromLatin1(transaction_str.c_str());

    QUrl url(url_path);
    QNetworkRequest request(url);

    mgr->get(request);
}

void MainWindow::getTransactionCommand_bit(std::string transaction_str)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)), this, SLOT(getTransactionFinish(QNetworkReply*)));

    QString url_path = "https://chain.so/api/v2/get_tx_inputs/DOGE/";//"https://dogechain.info/api/v1/transaction/";
    url_path += QString::fromLatin1(transaction_str.c_str());

    QUrl url(url_path);
    QNetworkRequest request(url);

    mgr->get(request);
}

void MainWindow::getTransactionFinish(QNetworkReply *rep)
{
    QByteArray bts = rep->readAll();
    QString str(bts);
    QMessageBox::information(this, "Get Transcation", str, "ok");
}


//bool client::settxfee( const std::string& fee_value)
//{
//    std::stringstream ss;
//    ss << "{\"jsonrpc\": \"1.0\", \"id\":\"1\", \"method\": \"settxfee\", \"params\": [";
//    ss << fee_value;
//    ss << "] }";
//    return my->request(ss.str()).get<std::string>("result")=="true";
//}











