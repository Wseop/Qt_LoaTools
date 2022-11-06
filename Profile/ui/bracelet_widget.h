#ifndef BRACELET_WIDGET_H
#define BRACELET_WIDGET_H

#include <QWidget>

class Bracelet;
class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class BraceletWidget;
}

class BraceletWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BraceletWidget(QWidget *parent = nullptr, const Bracelet* bracelet = nullptr, QString iconUrl = "");
    ~BraceletWidget();

private:
    void requestIcon(QString iconUrl);
    void setText();
    void setStyleSheet();

private:
    Ui::BraceletWidget *ui;
    const Bracelet* mBracelet;
    QNetworkAccessManager* mNetworkManager = nullptr;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // BRACELET_WIDGET_H
