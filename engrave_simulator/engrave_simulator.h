#ifndef ENGRAVE_SIMULATOR_H
#define ENGRAVE_SIMULATOR_H

#include <QWidget>
#include <QStringList>
#include <QMap>

#include "engrave.h"

namespace Ui {
class EngraveSimulator;
}

class EngraveSimulator : public QWidget
{
    Q_OBJECT

private:
    EngraveSimulator();
    ~EngraveSimulator();

public:
    static EngraveSimulator* getInstance();
    static void destroyInstance();

private:
    void initMap();
    void initUI();
    void initConnect();

    bool validateAccValue();
    void addEngraveLayout(QString engrave, int value);
    void clearEngraveLayout();

private:
    const int MAX_EQUIP = 12;
    const int MAX_ACC = 6;
    const int MAX_ACC_PENALTY = 3;
    const int MAX_STONE = 10;

    const int INDEX_ACC_START = 0;
    const int INDEX_ACC_END = 9;
    const int INDEX_STONE_START = 10;
    const int INDEX_STONE_END = 11;
    const int INDEX_EQUIP_START = 12;
    const int INDEX_EQUIP_END = 13;

    Ui::EngraveSimulator *ui;
    class QHBoxLayout* mEngraveLayout;

    Engrave mEngrave;
    QMap<int, class QLineEdit*> mEngraveLEMap;
    QMap<int, class QComboBox*> mPenaltyCBMap;
    QMap<int, class QSpinBox*> mEngraveSPBMap;
    QMap<int, class QSpinBox*> mPenaltySPBMap;

    QList<class QLabel*> mLabelList;
    QList<class QVBoxLayout*> mVBoxLayoutList;

    static EngraveSimulator* m_pEngraveSimulator;

private slots:
    void slotClearInput();
    void slotUpdateResult();
};

#endif // ENGRAVE_SIMULATOR_H
