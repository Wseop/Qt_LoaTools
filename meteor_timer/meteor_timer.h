#ifndef METEOR_TIMER_H
#define METEOR_TIMER_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class MeteorTimer;
}

class MeteorTimer : public QWidget
{
    Q_OBJECT

    const int MAX_COUNT = 100;

public:
    explicit MeteorTimer(QWidget *parent = nullptr);
    ~MeteorTimer();

private:
    Ui::MeteorTimer *ui;
    QTimer* mTimer;
    int mCount;

private:
    void initConnect();

private slots:
    void slotStartTimer();
    void slotResetTimer();
    void slotUpdateTimer();
};

#endif // METEOR_TIMER_H
