#ifndef CLASS_SELECTOR_H
#define CLASS_SELECTOR_H

#include <QWidget>

enum class ParentClass
{
    None,
    SettingAdviser,
    RankingBoard,
};

namespace Ui {
class ClassSelector;
}

class ClassSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ClassSelector(QWidget *parent = nullptr, ParentClass parentClass = ParentClass::None);
    ~ClassSelector();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void initStyleSheet();
    void initConnect();

private:
    Ui::ClassSelector *ui;

    QWidget* m_pParent = nullptr;
    ParentClass m_parentClass;
};

#endif // CLASS_SELECTOR_H
