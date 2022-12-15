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
    GemWidget(QWidget* pParent, const Gem* pGem);
    ~GemWidget();

private:
    void loadIcon();
    void setLabels();
    void setFonts();
    void setLayoutAlignments();

private:
    Ui::GemWidget *ui;
    QWidget* m_pParent;
    const Gem* m_pGem;
    class QNetworkAccessManager* m_pNetworkManager;
};

#endif // GEM_WIDGET_H
