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

public:
    static MeteorTimer* getInstance();
    static void destroyInstance();

private:
    Ui::MeteorTimer *ui;
    QTimer* mTimer;
    int mCount;

    static MeteorTimer* m_pMeteorTimer;

private:
    void initConnect();

private slots:
    void slotStartTimer();
    void slotResetTimer();
    void slotUpdateTimer();
};

#endif // METEOR_TIMER_H
