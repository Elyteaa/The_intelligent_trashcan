#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QByteArray>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_OpenLidButton_clicked();
    void on_lightsButton_clicked();
    void on_updateButton_clicked();
    void readSerial();
    void updateOpenedLCD(const QString);
    void updateFullLCD(const QString);
    void updateTrashCan(QString);
private:
    Ui::Dialog *ui;
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 10755;
    static const quint16 arduino_uno_product_id = 67;
    QByteArray serialData;
    QString serialBuffer;
    QString arduino_port_name;
    bool arduino_is_available;
};

#endif // DIALOG_H
