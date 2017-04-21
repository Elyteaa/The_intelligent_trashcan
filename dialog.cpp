#include "dialog.h"
#include "ui_dialog.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QDebug>
#include <QtWidgets>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->openedLCD->display("-----"); //initial printOut
    ui->fullLCD->display("-----");

    arduino_is_available = false;
    arduino_port_name = "";
    serialBuffer = "";
    arduino = new QSerialPort;

    /*
    qDebug() << "Number of available ports: " << QSerialPortInfo::availablePorts().length();
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier()){
            qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier();
        }
        qDebug() << "Has Product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier()){
            qDebug() << "Product ID: " << serialPortInfo.productIdentifier();
        }
    }
    */

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier())        {
            if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id) {
                if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                    arduino_port_name = serialPortInfo.portName();
                    arduino_is_available = true;
                }
            }
        }
    }
    if(arduino_is_available){
        //open and configure the serialport
        //order matters
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite); //could be ReadOnly, WriteOnly or ReadWrite
        arduino->setBaudRate(QSerialPort::Baud9600); //has to match Arduino sketch
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(readSerial()));
    } else {
        //give error message
        QMessageBox::warning(this, "Port error", "Couldn't find the Arduino!");
    }
}

Dialog::~Dialog() //destructor
{
    if(arduino->isOpen()){
        arduino->close();
    }
    delete ui;
}

void Dialog::on_OpenLidButton_clicked()
{
    //ui->LidStatusLabel->setText(QString("Opening"));
    Dialog::updateTrashCan(QString("pressed"));
}

void Dialog::on_lightsButton_clicked()
{
    Dialog::updateTrashCan(QString("lights"));
}

void Dialog::readSerial(){
    Dialog::updateTrashCan(QString("update")); //updating info on the port
    //qDebug() << "The serialport works!";
    //QByteArray serialData = arduino->readAll();
    //QString temp = QString::fromStdString(serialData.toStdString());
    //serialData = arduino->readAll();
    //serialBuffer += QString::fromStdString(serialData.toStdString());
    //qDebug() << serialBuffer;

    QStringList bufferSplit = serialBuffer.split(',');
    if(bufferSplit.length() < 2){
        serialData = arduino->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());
        qDebug() << bufferSplit;
        serialData.clear();
    } else {
        serialBuffer = "";
        //bufferSplit[0] is the first value
        if((bufferSplit[0] != "") && (bufferSplit[1] != "")){
            qDebug() << "Transfered to LCD";
            Dialog::updateOpenedLCD(bufferSplit[0]);
            Dialog::updateFullLCD(bufferSplit[1]);
        } else {QMessageBox::warning(this, "Error with data", "Unsuccessful data transfering through the port, please try again!");};
    }
}

void Dialog::updateOpenedLCD(const QString sensor_reading){
    ui->openedLCD->display(sensor_reading);
}

void Dialog::updateFullLCD(const QString sensor_reading){
    ui->fullLCD->display(sensor_reading);
}

void Dialog::updateTrashCan(QString command){
    if(arduino->isWritable()){
        arduino->write(command.toStdString().c_str());
    } else {
        qDebug() << "Couldn't write to serial!";
    }
}

void Dialog::on_updateButton_clicked()
{
    //Dialog::updateTrashCan(QString("update"));
    readSerial();
}
