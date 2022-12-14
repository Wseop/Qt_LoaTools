#ifndef GEM_WIDGET_H
#define GEM_WIDGET_H

#include <QWidget>

class Gem;

namespace Ui {
class GemWidget;
}

class GemWidget : public QWidget
{
    Q_OBJECT

public:
    GemWidget(QWidget* parent, const Gem* gem);
    ~GemWidget();

private:
    void setFonts();
    void setAlignments();
    void setStyleSheets();
    void setTexts();
    void requestIcon();

private:
    Ui::GemWidget *ui;
    QWidget* m_pParent;
    const Gem* m_pGem;
    class QNetworkAccessManager* m_pNetworkManager;
};

#endif // GEM_WIDGET_H
