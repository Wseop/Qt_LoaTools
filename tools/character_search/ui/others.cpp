#include "others.h"
#include "ui_others.h"
#include "tools/character_search/character_search.h"

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

    initButtons();
}

Others::~Others()
{
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

    const QStringList& servers = serverToOthers.keys();
    for (const QString& server : servers)
    {
        movePos(MoveType::Server);
        // TODO. memory release
        QLabel* serverLabel = new QLabel("@ " + server);
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
    QString btnText = QString("%1 Lv.%2 [%3]\n%4").arg(classToStr(cls)).arg(level).arg(itemLevel).arg(name);
    QPushButton* btn = new QPushButton(btnText);
    m_btnToName[btn] = name;

    connect(btn, &QPushButton::pressed, this, [&](){
        CharacterSearch* parent = static_cast<CharacterSearch*>(m_pParent);
        parent->setEnabled(true);
        this->close();
        parent->changeCharacter(m_btnToName[static_cast<QPushButton*>(sender())]);
    });
    return btn;
}
