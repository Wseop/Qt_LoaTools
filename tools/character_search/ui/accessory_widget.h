#ifndef ACCESSORY_WIDGET_H
#define ACCESSORY_WIDGET_H

#include <QWidget>
#include <QLabel>

class Accessory;

namespace Ui {
class AccessoryWidget;
}

class AccessoryWidget : public QWidget
{
    Q_OBJECT

public:
    AccessoryWidget(QWidget* pParent, const Accessory* pAccessory);
    ~AccessoryWidget();

private:
    void loadIcon();
    void setQualityBar();
    void setLabels();
    QLabel* createLabel(QString labelText, QString labelColor);
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::AccessoryWidget *ui;
    QWidget* m_pParent;
    const Accessory* m_pAccessory;
    class QNetworkAccessManager* m_pNetworkManager;
    QList<QLabel*> m_labels;
};

#endif // ACCESSORY_WIDGET_H
