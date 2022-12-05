#ifndef ENGRAVE_WIDGET_H
#define ENGRAVE_WIDGET_H

#include <QWidget>

class Engrave;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;

namespace Ui {
class EngraveWidget;
}

class EngraveWidget : public QWidget
{
    Q_OBJECT

public:
    EngraveWidget(const QMap<QString, int>& engraveValues, const QMap<QString, int>& penaltyValues);
    ~EngraveWidget();

private:
    void initFont();

    void createEngraveLayouts(const QStringList& engraves);
    void createPenaltyLayouts(const QStringList& penalties);
    QHBoxLayout* createEngraveLayout(QString iconPath, QString engrave, int level);

private:
    Ui::EngraveWidget *ui;

    QMap<QString, int> m_engraveValues;
    QMap<QString, int> m_penaltyValues;
    QList<QLabel*> m_labels;
    QList<QHBoxLayout*> m_engraveLayouts;
    QList<QHBoxLayout*> m_penaltyLayouts;
};

#endif // ENGRAVE_WIDGET_H
