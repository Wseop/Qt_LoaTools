#include "character_list.h"
#include "ui_character_list.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCloseEvent>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

CharacterList::CharacterList(QWidget *parent, QWidget* ret) :
    QWidget(parent),
    ui(new Ui::CharacterList),
    mParent(ret),
    mNetworkItemLevel(new QNetworkAccessManager())
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/resources/Home.ico"));
    this->setWindowTitle("보유 캐릭터 목록");

    initConnect();
}

CharacterList::~CharacterList()
{
    delete ui;
    delete mNetworkItemLevel;

    const QStringList& keys = mNameTitle.keys();
    for (const QString& key : keys)
        delete mNameTitle[key];

    for (QWidget* widget : mWidgetList)
        delete widget;
}

void CharacterList::initConnect()
{
    connect(mNetworkItemLevel, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotParseItemLevel(QNetworkReply*)));
}

void CharacterList::addCharacter(QString server, QString name, QString cls)
{
    CharacterTitle* title = new CharacterTitle();
    title->name = name;
    title->cls = cls;

    mServerCharacterList[server].append(title);
    mNameTitle[name] = title;

    QString url = QString("https://lostark.game.onstove.com/Profile/Character/%1").arg(title->name);

    QNetworkRequest request((QUrl(url)));
    mNetworkItemLevel->get(request);
}

void CharacterList::updateUI()
{
    if (mIsUIUpdated)
        return;
    mIsUIUpdated = true;

    QStringList serverList = mServerCharacterList.keys();
    for (const QString& server : serverList)
    {
        QLabel* serverLabel = new QLabel(server);
        serverLabel->setFont(QFont("나눔스퀘어 네오 Bold", 12));
        serverLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        serverLabel->setStyleSheet("QLabel { color: #B178FF }");
        ui->vLayoutCharacterList->addWidget(serverLabel);
        mWidgetList.append(serverLabel);

        const QList<CharacterTitle*>& characterList = mServerCharacterList[server];
        QHBoxLayout* layout;
        for (int i = 0; i < characterList.size(); i++)
        {
            if (i % 3 == 0)
            {
                layout = new QHBoxLayout();
                ui->vLayoutCharacterList->addLayout(layout);
                mLayoutList.append(layout);
            }
            QPushButton* pbTitle = new QPushButton();
            const CharacterTitle* title = characterList[i];
            QString text = QString("%1 %2 %3").arg(title->cls, title->name, title->itemLevel);
            pbTitle->setText(text);
            pbTitle->setFont(QFont("나눔스퀘어 네오 Regular", 10));
            layout->addWidget(pbTitle);
            mWidgetList.append(pbTitle);
        }
    }
}

void CharacterList::closeEvent(QCloseEvent *event)
{
    mParent->setEnabled(true);
    event->accept();
}

void CharacterList::slotParseItemLevel(QNetworkReply* reply)
{
    QString profile = reply->readAll();
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
    mNameTitle[name]->itemLevel = itemLevel;
}
