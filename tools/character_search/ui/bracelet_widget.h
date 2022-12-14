#ifndef BRACELET_WIDGET_H
#define BRACELET_WIDGET_H

#include <QWidget>
#include <QLabel>

class Bracelet;

namespace Ui {
class BraceletWidget;
}

class BraceletWidget : public QWidget
{
    Q_OBJECT

public:
    BraceletWidget(QWidget* parent, const Bracelet* bracelet);
    ~BraceletWidget();

private:
    void setFonts();
    void setAlignments();
    void setStyleSheets();
    void requestIcon();
    void setTexts();
    void addEffectLabels();

private:
    Ui::BraceletWidget *ui;
    QWidget* m_pParent;
    const Bracelet* m_pBracelet;
    class QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_effectLabels;
};

#endif // BRACELET_WIDGET_H
