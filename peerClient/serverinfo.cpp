#include "serverinfo.h"
#include "ui_serverinfo.h"

ServerInfo* ServerInfo::_serverInfo = 0;
ServerInfo::ServerInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerInfo)
{
    ui->setupUi(this);

    ui->lineEditServerPort->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    ui->lineEditMyPort->setValidator(new QRegExpValidator(QRegExp("\\d+")));
    ui->lineEditUserName->hide();
    ui->label_3->hide();
}

ServerInfo::~ServerInfo()
{
    delete ui;
}

void ServerInfo::on_pushButtonOk_pressed()
{
    if(ui->lineEditServerIp->text().isEmpty() || ui->lineEditServerPort->text().isEmpty() ||
            ui->lineEditMyIp->text().isEmpty() || ui->lineEditMyPort->text().isEmpty() )
         QMessageBox::information(this,"ERROR","IP or Port Number is missing");
    else
         {
            //_userName = ui->lineEditUserName->text();
            _serverIp = ui->lineEditServerIp->text();
            _serverPort = ui->lineEditServerPort->text().toInt();
            _myIp = ui->lineEditMyIp->text();
            _myPort = ui->lineEditMyPort->text().toInt();

            accept();
        }
}
