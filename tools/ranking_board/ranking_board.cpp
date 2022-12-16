#include "ranking_board.h"
#include "ui_ranking_board.h"
#include "db/db_request.h"
#include "font/font_manager.h"

#include <QJsonObject>
#include <QThread>

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

    QThread* dbThread = new QThread();
    m_pDBRequest->moveToThread(dbThread);
    dbThread->start();

    requestAllCharacters();
}

RankingBoard::~RankingBoard()
{
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
    FontManager* fontManager = FontManager::getInstance();
    QFont fontNanumBold10 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 10);
    QFont fontNanumBold12 = fontManager->getFont(FontFamily::NanumSquareNeoBold, 12);

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
    for (QLabel* label : m_labels)
    {
        ui->gridRankingBoard->removeWidget(label);
        delete label;
    }
    m_labels.clear();
}

void RankingBoard::requestAllCharacters()
{
    ui->lbSelectedClass->setText("전체 랭킹");
    disableInput();

    connect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleCharacters(QVariantList)));
    //emit m_pDBRequest->requestAllDocuments(Collection::Character, -1, "Level");
}

QLabel *RankingBoard::createLabel(QString text)
{
    QLabel* label = new QLabel(text);
    label->setFixedSize(150, 50);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    return label;
}

QString RankingBoard::getCharacterName(int index)
{
    return m_characters[index].toJsonObject().find("Name")->toString();
}

double RankingBoard::getCharacterItemLevel(int index)
{
    return m_characters[index].toJsonObject().find("Level")->toDouble();
}

Class RankingBoard::getCharacterClass(int index)
{
    return strToClass(m_characters[index].toJsonObject().find("Class")->toString());
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

            QLabel* lbRank = createLabel(QString("%1").arg(m_rankIndex + 1));
            lbRank->setFont(fontNanumBold10);
            QLabel* lbName = createLabel(name);
            lbName->setFont(fontNanumBold10);
            QLabel* lbItemLevel = createLabel(QString("%1").arg(itemLevel));
            lbItemLevel->setFont(fontNanumBold10);
            QLabel* lbClass = createLabel(classToStr(cls));
            lbClass->setFont(fontNanumBold10);

            m_labels.append(lbRank);
            m_labels.append(lbName);
            m_labels.append(lbItemLevel);
            m_labels.append(lbClass);

            ui->gridRankingBoard->addWidget(lbRank, m_rankIndex, 0);
            ui->gridRankingBoard->addWidget(lbName, m_rankIndex, 1);
            ui->gridRankingBoard->addWidget(lbItemLevel, m_rankIndex, 2);
            ui->gridRankingBoard->addWidget(lbClass, m_rankIndex, 3);

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

void RankingBoard::slotHandleCharacters(QVariantList characters)
{
    disconnect(m_pDBRequest, SIGNAL(finished(QVariantList)), this, SLOT(slotHandleCharacters(QVariantList)));

    m_characters = characters;
    enableInput();
    renderCharacters(true);
}
