#ifndef CLASS_SELECTOR_H
#define CLASS_SELECTOR_H

#include <QWidget>

namespace Ui {
class ClassSelector;
}

class ClassSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ClassSelector(QWidget *parent = nullptr, QWidget* ret = nullptr);
    ~ClassSelector();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void initStyleSheet();
    void initConnect();

private:
    Ui::ClassSelector *ui;

    QWidget* m_pParent = nullptr;
};

#endif // CLASS_SELECTOR_H
