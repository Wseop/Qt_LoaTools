#ifndef ACC_WIDGET_H
#define ACC_WIDGET_H

#include <QWidget>

class Accessory;
class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class AccWidget;
}

class AccWidget : public QWidget
{
    Q_OBJECT

public:
    AccWidget(const Accessory* acc = nullptr, QString iconUrl = "");
    ~AccWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();
    QString getQualColor();

private:
    Ui::AccWidget *ui;
    const Accessory* m_pAcc;
    QNetworkAccessManager* m_pNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // ACC_WIDGET_H
