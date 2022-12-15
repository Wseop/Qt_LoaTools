#ifndef ENGRAVE_WIDGET_H
#define ENGRAVE_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

class Engrave;

namespace Ui {
class EngraveWidget;
}

class EngraveWidget : public QWidget
{
    Q_OBJECT

public:
    EngraveWidget(QWidget* pParent, const Engrave* pEngrave);
    ~EngraveWidget();

private:
    void setEngraveLayouts();
    QHBoxLayout* createHLayout();
    QLabel* createIconLabel(QString iconPath);
    QLabel* createNameLabel(QString engrave);
    QLabel* createLevelLabel(int level);
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::EngraveWidget *ui;
    QWidget* m_pParent;
    const Engrave* m_pEngrave;
    QList<QHBoxLayout*> m_hLayouts;
    QList<QLabel*> m_labels;
};

#endif // ENGRAVE_WIDGET_H
