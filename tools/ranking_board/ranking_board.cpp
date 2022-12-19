#include "ranking_board.h"
#include "ui_ranking_board.h"
#include "db/db_request.h"
#include "font/font_manager.h"

#include <QJsonObject>
#include <QThread>

void loadCharacters(DBRequest* pDbRequest)
{
    pDbRequest->findDocuments(Collection::Character, -1, "Level");
}

RankingBoard* RankingBoard::m_pRankingBoard = nullptr;

RankingBoard::RankingBoard() :
    ui(new Ui::RankingBoard),
    RENDER_COUNT(100),
    m_pDBRequest(new DBRequest()),
    m_rankIndex(0),
    m_characterIndex(0),
    m_selectedItemLevel(1655),
    m_selectedClass(Class::Size),
    m_pClassSelector(new ClassSelector(this, ParentClass::RankingBoard))
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("캐릭터 랭킹");
    this->showMaximized();
    ui->lbInfo->hide();

    initAlignment();
    initFont();
    initConnect();

    requestAllCharacters();
}

RankingBoard::~RankingBoard()
{
    delete m_pDBRequest;
    delete ui;
    destroyInstance();
}

void RankingBoard::setSelectedClass(Class cls)
{
    m_selectedClass = cls;    
    renderCharacters(true);
}

RankingBoard *RankingBoard::getInstance()
{
    if (m_pRankingBoard == nullptr)
        m_pRankingBoard = new RankingBoard();

    return m_pRankingBoard;
}

void RankingBoard::destroyInstance()
{
    if (m_pRankingBoard == nullptr)
        return;

    delete m_pRankingBoard;
    m_pRankingBoard = nullptr;
}

void RankingBoard::initAlignment()
{
    ui->vLayoutRankingBoard->setAlignment(Qt::AlignHCenter);
    ui->hLayoutSelector->setAlignment(Qt::AlignHCenter);
    ui->vLayoutScrollArea->setAlignment(Qt::AlignHCenter);
    ui->gridRankingLabels->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    ui->gridRankingBoard->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
}

void RankingBoard::initFont()
{
    FontManager* pFontManager = FontManager::getInstance();
    QFont fontNanumBold10 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont fontNanumBold12 = pFontManager->getFont(FontFamily::NanumSquareNeoBold, 12);

    ui->lbSelectedClass->setFont(fontNanumBold10);
    ui->lbSelectItemLevel->setFont(fontNanumBold10);

    ui->lbClass->setFont(fontNanumBold12);
    ui->lbItemLevel->setFont(fontNanumBold12);
    ui->lbName->setFont(fontNanumBold12);
    ui->lbRank->setFont(fontNanumBold12);
    ui->pbSelectAllClass->setFont(fontNanumBold12);
    ui->pbSelectClass->setFont(fontNanumBold12);
    ui->pbRenderMore->setFont(fontNanumBold12);
    ui->lbInfo->setFont(fontNanumBold12);
}

void RankingBoard::initConnect()
{
    connect(ui->hSliderSelectItemLevel, SIGNAL(sliderMoved(int)), this, SLOT(slotSetSelectItemLevel(int)));
    connect(ui->hSliderSelectItemLevel, SIGNAL(valueChanged(int)), this, SLOT(slotItemLevelChanged()));

    connect(ui->pbSelectAllClass, SIGNAL(pressed()), this, SLOT(slotRenderAll()));
    connect(ui->pbSelectClass, SIGNAL(pressed()), this, SLOT(slotShowClassSelector()));
    connect(ui->pbRenderMore, SIGNAL(pressed()), this, SLOT(slotRenderMore()));

    connect(m_pDBRequest, &DBRequest::findDocumentsFinished, this, &RankingBoard::slotSetCharacters);
}

void RankingBoard::disableInput()
{
    ui->pbRenderMore->setDisabled(true);
    ui->pbSelectClass->setDisabled(true);
    ui->pbSelectAllClass->setDisabled(true);
    ui->hSliderSelectItemLevel->setDisabled(true);
    ui->scrollArea->hide();
    ui->lbInfo->show();
}

void RankingBoard::enableInput()
{
    ui->pbRenderMore->setEnabled(true);
    ui->pbSelectClass->setEnabled(true);
    ui->pbSelectAllClass->setEnabled(true);
    ui->hSliderSelectItemLevel->setEnabled(true);
    ui->scrollArea->show();
    ui->lbInfo->hide();
}

void RankingBoard::clear()
{
    // release labels
    for (QLabel* pLabel : m_labels)
    {
        ui->gridRankingBoard->removeWidget(pLabel);
        delete pLabel;
    }
    m_labels.clear();
}

void RankingBoard::requestAllCharacters()
{
    ui->lbSelectedClass->setText("전체 랭킹");
    disableInput();

    m_pThread = QThread::create(loadCharacters, m_pDBRequest);
    m_pThread->start();
}

QLabel* RankingBoard::createLabel(QString text)
{
    QLabel* pLabel = new QLabel(text);
    pLabel->setFixedSize(150, 50);
    pLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_labels.append(pLabel);
    return pLabel;
}

QString RankingBoard::getCharacterName(int index)
{
    return m_characters[index].toObject().find("Name")->toString();
}

double RankingBoard::getCharacterItemLevel(int index)
{
    return m_characters[index].toObject().find("Level")->toDouble();
}

Class RankingBoard::getCharacterClass(int index)
{
    return strToClass(m_characters[index].toObject().find("Class")->toString());
}

void RankingBoard::renderCharacters(bool bInitialize)
{
    if (bInitialize)
    {
        clear();
        m_rankIndex = 0;
        m_characterIndex = 0;

        if (m_selectedClass == Class::Size)
            ui->lbSelectedClass->setText("전체 랭킹");
        else
            ui->lbSelectedClass->setText(classToStr(m_selectedClass));
    }

    QFont fontNanumBold10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10);
    int i = 0;
    while (i < RENDER_COUNT && m_characterIndex < m_characters.size())
    {
        Class cls = getCharacterClass(m_characterIndex);
        double itemLevel = getCharacterItemLevel(m_characterIndex);
        if ((m_selectedClass == Class::Size || m_selectedClass == cls) && itemLevel <= (double)m_selectedItemLevel)
        {
            QString name = getCharacterName(m_characterIndex);

            QLabel* pLbRank = createLabel(QString("%1").arg(m_rankIndex + 1));
            pLbRank->setFont(fontNanumBold10);
            QLabel* pLbName = createLabel(name);
            pLbName->setFont(fontNanumBold10);
            QLabel* pLbItemLevel = createLabel(QString("%1").arg(itemLevel));
            pLbItemLevel->setFont(fontNanumBold10);
            QLabel* pLbClass = createLabel(classToStr(cls));
            pLbClass->setFont(fontNanumBold10);

            ui->gridRankingBoard->addWidget(pLbRank, m_rankIndex, 0);
            ui->gridRankingBoard->addWidget(pLbName, m_rankIndex, 1);
            ui->gridRankingBoard->addWidget(pLbItemLevel, m_rankIndex, 2);
            ui->gridRankingBoard->addWidget(pLbClass, m_rankIndex, 3);

            m_rankIndex++;
            i++;
        }
        m_characterIndex++;
    }
}

void RankingBoard::slotSetSelectItemLevel(int value)
{
    QString text = QString("아이템 레벨\n%1 이하").arg(value);
    ui->lbSelectItemLevel->setText(text);
}

void RankingBoard::slotItemLevelChanged()
{
    m_selectedItemLevel = ui->hSliderSelectItemLevel->value();
    slotSetSelectItemLevel(m_selectedItemLevel);
    renderCharacters(true);
}

void RankingBoard::slotShowClassSelector()
{
    this->setDisabled(true);
    m_pClassSelector->show();
}

void RankingBoard::slotRenderMore()
{
    renderCharacters(false);
}

void RankingBoard::slotRenderAll()
{
    m_selectedClass = Class::Size;
    renderCharacters(true);
}

void RankingBoard::slotSetCharacters(QJsonArray characters)
{
    m_characters = characters;
    enableInput();
    renderCharacters(true);
}
