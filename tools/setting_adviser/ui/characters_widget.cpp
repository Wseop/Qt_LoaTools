#include "characters_widget.h"
#include "ui_characters_widget.h"
#include "tools/character_search/character_search.h"
#include "font/font_manager.h"

#include <QCloseEvent>

CharactersWidget::CharactersWidget(QStringList characters) :
    ui(new Ui::CharactersWidget),
    row(0), col(0)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 목록");

    setButtons(characters);
    setFonts();
}

CharactersWidget::~CharactersWidget()
{
    for (QPushButton* pBtn : m_btns)
        delete pBtn;
    m_btns.clear();

    delete ui;
}

void CharactersWidget::closeEvent(QCloseEvent* event)
{
    event->accept();
}

void CharactersWidget::setButtons(const QStringList& characters)
{
    for (const QString& character : characters)
    {
        QPushButton* pButton = createButton(character);
        ui->gridCharacters->addWidget(pButton, row, col);
        col++;
        if (col == MAX_COL)
        {
            col = 0;
            row++;
        }
    }
}

QPushButton* CharactersWidget::createButton(const QString name)
{
    QPushButton* pButton = new QPushButton(name);
    m_btns.append(pButton);
    connect(pButton, &QPushButton::pressed, this, [&, pButton](){
        CharacterSearch::getInstance()->changeCharacter(pButton->text());
        if (CharacterSearch::getInstance()->isHidden())
            CharacterSearch::getInstance()->show();
        this->close();
    });
    return pButton;
}

void CharactersWidget::setFonts()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont nanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);

    for (QPushButton* pButton : m_btns)
    {
        pButton->setFont(nanumBold10);
    }
}
