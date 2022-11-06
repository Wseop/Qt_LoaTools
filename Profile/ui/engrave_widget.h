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
    explicit EngraveWidget(QWidget *parent = nullptr, const Engrave* engrave = nullptr);
    ~EngraveWidget();

private:
    void createEngraveLayouts(const QStringList& engraves);
    void createPenaltyLayouts(const QStringList& penalties);
    QHBoxLayout* createEngraveLayout(QString iconPath, QString engrave, int level);

private:
    Ui::EngraveWidget *ui;
    const Engrave* mEngrave;
    QList<QLabel*> mLabels;
    QList<QHBoxLayout*> mEngraveLayouts;
    QList<QHBoxLayout*> mPenaltyLayouts;
};

#endif // ENGRAVE_WIDGET_H
