#include "character_list.h"
#include "ui_character_list.h"
#include "profile.h"
#include "font/font_manager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

CharacterList::CharacterList(QWidget *parent) :
    ui(new Ui::CharacterList),
    m_pParent(parent),
    m_pNetworkItemLevel(new QNetworkAccessManager())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("보유 캐릭터 목록");

    initConnect();
}

CharacterList::~CharacterList()
{
    delete ui;
    delete m_pNetworkItemLevel;

    QStringList keys = m_nameButton.keys();
    for (QString& key : keys)
        delete m_nameButton[key];
    m_nameButton.clear();

    for (QLabel* label : m_labelList)
        delete label;
    m_labelList.clear();

    keys = m_serverLayout.keys();
    for (QString& key : keys)
        delete m_serverLayout[key];
    m_serverLayout.clear();

    keys = m_nameTitle.keys();
    for (QString& key : keys)
        delete m_nameTitle[key];
    m_nameTitle.clear();
}

void CharacterList::initConnect()
{
    connect(m_pNetworkItemLevel, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotParseItemLevel(QNetworkReply*)));
}

void CharacterList::addCharacter(QString server, QString name, QString cls)
{
    CharacterTitle* title = new CharacterTitle();
    title->name = name;
    title->server = server;
    title->cls = cls;
    m_nameTitle[name] = title;

    if (!m_serverLayout.contains(server))
    {
        // server명 레이아웃이 없으면 새로 추가
        QGridLayout* layout = new QGridLayout();
        m_serverLayout[server] = layout;

        QLabel* label = new QLabel(server);
        m_labelList.append(label);
        QFont fontNanumExtraBold12 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoExtraBold, 12);
        label->setFont(fontNanumExtraBold12);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setFixedHeight(30);
        label->setStyleSheet("QLabel { color: #B178FF; background-color: #444A5B }");

        layout->addWidget(label, 0, 0, 1, 3);
        ui->vLayoutCharacterList->addLayout(layout);

        m_serverGridPos[server] = GridPos{1, 0};
    }

    // level 정보 request
    QString url = QString("https://lostark.game.onstove.com/Profile/Character/%1").arg(title->name);
    QNetworkRequest request((QUrl(url)));
    m_pNetworkItemLevel->get(request);
}

void CharacterList::closeEvent(QCloseEvent *event)
{
    m_pParent->setEnabled(true);
    event->accept();
}

void CharacterList::movePos(GridPos &pos)
{
    pos.col += 1;
    if (pos.col > MAX_COL)
    {
        pos.row += 1;
        pos.col = 0;
    }
}

// level 정보 update 후 ui 추가
void CharacterList::slotParseItemLevel(QNetworkReply* reply)
{
    QString profile = reply->readAll();

    // 존재하지 않는 캐릭터 skip
    if (profile.indexOf("$.Profile = ") == -1)
        return;

    QString itemLevel;
    qsizetype startIndex, endIndex;
    startIndex = profile.indexOf("level-info2__expedition");
    startIndex = profile.indexOf("</small>", startIndex) + 8;
    endIndex = profile.indexOf("<small>", startIndex);
    itemLevel = profile.sliced(startIndex, endIndex - startIndex);
    startIndex = endIndex + 7;
    endIndex = profile.indexOf("</small>", startIndex);
    itemLevel += profile.sliced(startIndex, endIndex - startIndex);

    QString name = reply->url().path().remove("/Profile/Character/");
    CharacterTitle* title = m_nameTitle[name];
    title->itemLevel = itemLevel;

    QString text = QString("%1 [%2]\n%3").arg(title->cls, title->itemLevel, title->name);
    QPushButton* btn = new QPushButton(text);
    QFont fontNanumRegular10 = FontManager::getInstance()->getFont(FontFamily::NanumSquareNeoRegular, 10);
    btn->setFont(fontNanumRegular10);
    m_nameButton[name] = btn;

    connect(btn, SIGNAL(pressed()), this, SLOT(slotSearchCharacter()));

    GridPos& pos = m_serverGridPos[title->server];
    m_serverLayout[title->server]->addWidget(btn, pos.row, pos.col, 1, 1);
    movePos(pos);
}

void CharacterList::slotSearchCharacter()
{
    QPushButton* btn = static_cast<QPushButton*>(sender());

    m_pParent->setEnabled(true);
    Profile* profile = static_cast<Profile*>(m_pParent);
    profile->profileRequest(m_nameButton.key(btn));

    this->close();
}