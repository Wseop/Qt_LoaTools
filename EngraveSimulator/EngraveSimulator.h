#ifndef ENGRAVESIMULATOR_H
#define ENGRAVESIMULATOR_H

#include <QWidget>
#include <QStringList>
#include <QMap>

namespace Ui {
class EngraveSimulator;
}

class EngraveSimulator : public QWidget
{
    Q_OBJECT

public:
    explicit EngraveSimulator(QWidget *parent = nullptr);
    ~EngraveSimulator();

private:
    void loadEngraveList();

    void initMap();
    void initUI();
    void initConnect();

    bool isValidEngrave(QString engrave);
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

    QStringList mEngraveList;
    QStringList mPenaltyList;
    QMap<int, class QLineEdit*> mEngraveLEMap;
    QMap<int, class QComboBox*> mPenaltyCBMap;
    QMap<int, class QSpinBox*> mEngraveSPBMap;
    QMap<int, class QSpinBox*> mPenaltySPBMap;

    QMap<QString, int> mEngraveValueMap;
    QMap<QString, int> mPenaltyValueMap;

    QList<class QLabel*> mLabelList;
    QList<class QVBoxLayout*> mVBoxLayoutList;

private slots:
    void slotClearInput();
    void slotUpdateResult();
};

#endif // ENGRAVESIMULATOR_H
