#include "others.h"
#include "ui_others.h"
#include "tools/character_search/character_search.h"
#include "font/font_manager.h"

#include <QCloseEvent>
#include <QLabel>

Others::Others(QWidget *parent, const QList<Other>& others) :
    ui(new Ui::Others),
    m_pParent(parent),
    m_others(others),
    m_gridPos({-1, 0})
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("보유 캐릭터");
    ui->gridOthers->setAlignment(Qt::AlignTop);

    initButtons();
}

Others::~Others()
{
    const QList<QPushButton*> btns = m_btnToName.keys();
    for (QPushButton* btn : btns)
        delete btn;
    m_btnToName.clear();

    for (QLabel* serverLabel : m_serverLabels)
        delete serverLabel;
    m_serverLabels.clear();

    delete ui;
}

void Others::closeEvent(QCloseEvent* event)
{
    m_pParent->setEnabled(true);
    event->accept();
}

void Others::initButtons()
{
    QMap<QString, QList<Other>> serverToOthers;
    for (const Other& other : m_others)
    {
        serverToOthers[other.server].append(other);
    }

    QFont nanumBold16 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 16);

    const QStringList& servers = serverToOthers.keys();
    for (const QString& server : servers)
    {
        movePos(MoveType::Server);
        QLabel* serverLabel = new QLabel("@ " + server);
        serverLabel->setStyleSheet("QLabel { color: #B178FF; background-color: #444A5B }");
        serverLabel->setFont(nanumBold16);
        serverLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->gridOthers->addWidget(serverLabel, m_gridPos.row, m_gridPos.col, 1, 3);
        movePos(MoveType::Server);

        const QList<Other>& others = serverToOthers[server];
        for (const Other& other : others)
        {
            QPushButton* btn = createButton(other.cls, other.characterLevel, other.itemLevel, other.characterName);
            ui->gridOthers->addWidget(btn, m_gridPos.row, m_gridPos.col, 1, 1);
            movePos(MoveType::Character);
        }
    }
}

void Others::movePos(MoveType type)
{
    if (type == MoveType::Server)
    {
        // new server : move row
        m_gridPos.row++;
        m_gridPos.col = 0;
    }
    else if (type == MoveType::Character)
    {
        // new character : move col
        m_gridPos.col++;
        if (m_gridPos.col >= MAX_COL)
        {
            m_gridPos.row++;
            m_gridPos.col = 0;
        }
    }
}

QPushButton* Others::createButton(Class cls, int level, double itemLevel, QString name)
{
    QFont nanumBold10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoBold, 10);

    QString btnText = QString("%1 Lv.%2 [%3]\n%4").arg(classToStr(cls)).arg(level).arg(itemLevel).arg(name);
    QPushButton* btn = new QPushButton(btnText);
    btn->setFont(nanumBold10);
    m_btnToName[btn] = name;

    connect(btn, &QPushButton::pressed, this, [&](){
        CharacterSearch* parent = static_cast<CharacterSearch*>(m_pParent);
        parent->setEnabled(true);
        this->close();
        parent->changeCharacter(m_btnToName[static_cast<QPushButton*>(sender())]);
    });
    return btn;
}
