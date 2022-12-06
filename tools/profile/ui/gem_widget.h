#ifndef GEM_WIDGET_H
#define GEM_WIDGET_H

#include <QWidget>

class Gem;
class QNetworkAccessManager;
class QNetworkReply;

namespace Ui {
class GemWidget;
}

class GemWidget : public QWidget
{
    Q_OBJECT

public:
    GemWidget(const Gem* gem = nullptr, QString iconUrl = "");
    ~GemWidget();

private:
    void requestIcon(QString iconUrl);
    void setLevel();
    void setName();
    void setEffect();

private:
    Ui::GemWidget *ui;
    const Gem* m_pGem;
    QNetworkAccessManager* m_pNetworkManager;

private slots:
    void slotUpdateIcon(QNetworkReply* reply);
};

#endif // GEM_WIDGET_H
