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
    EngraveWidget(QWidget* parent, const Engrave* engrave);
    ~EngraveWidget();

private:
    void setFonts();
    void setAlignments();
    void setStyleSheets();
    void addEngraveLayouts();
    QLabel* createIcon(QString engrave);
    QLabel* createName(QString engrave);
    QLabel* createLevel(int level);

private:
    Ui::EngraveWidget *ui;
    QWidget* m_pParent;
    const Engrave* m_pEngrave;
    QList<QHBoxLayout*> m_hLayouts;
    QList<QLabel*> m_engraveLabels;
    QList<QLabel*> m_penaltyLabels;
};

#endif // ENGRAVE_WIDGET_H
