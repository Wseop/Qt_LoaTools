#ifndef CHARACTERS_WIDGET_H
#define CHARACTERS_WIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class CharactersWidget;
}

class CharactersWidget : public QWidget
{
    Q_OBJECT

public:
    CharactersWidget(QStringList characters);
    ~CharactersWidget();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setButtons(const QStringList& characters);
    QPushButton* createButton(const QString name);
    void setFonts();

private:
    Ui::CharactersWidget *ui;

    const int MAX_COL = 5;
    int row, col;
    QList<QPushButton*> m_btns;
};

#endif // CHARACTERS_WIDGET_H
