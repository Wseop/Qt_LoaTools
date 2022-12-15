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
    BraceletWidget(QWidget* pParent, const Bracelet* pBracelet);
    ~BraceletWidget();

private:
    void loadIcon();
    void setLabels();
    QLabel* createEffectLabels(QString labelText);
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::BraceletWidget *ui;
    QWidget* m_pParent;
    const Bracelet* m_pBracelet;
    class QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_labels;
};

#endif // BRACELET_WIDGET_H
