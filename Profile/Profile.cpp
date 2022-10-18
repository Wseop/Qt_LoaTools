#include "Profile.h"
#include "ui_profile.h"

#include <QMessageBox>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>

#ifdef QT_DEBUG
    #include <QFile>
#endif

Profile::Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Profile),
    mNetworkManager(new QNetworkAccessManager())
{
    ui->setupUi(this);
    ui->pbHome->setIcon(QIcon(":/resources/Home.ico"));
    ui->pbHome->setIconSize(QSize(30, 30));

    initUI();
    initConnect();
}

Profile::~Profile()
{
    delete ui;
}

void Profile::initUI()
{}

void Profile::initConnect()
{
    connect(ui->pbHome, SIGNAL(pressed()), this, SLOT(slotHome()));
    connect(ui->pbSearch, SIGNAL(pressed()), this, SLOT(slotProfileRequest()));
    connect(ui->leName, SIGNAL(returnPressed()), this, SLOT(slotProfileRequest()));
}

void Profile::slotHome()
{
    QWidget* parent = static_cast<QWidget*>(this->parent());
    parent->setFixedWidth(800);
    parent->setFixedHeight(600);

    this->close();
}

void Profile::slotProfileRequest()
{
    // 입력받은 캐릭터명으로 전투정보실에 request를 보냄
    QString name = ui->leName->text();
    QString url = QString("https://lostark.game.onstove.com/Profile/Character/%1").arg(name);
    QNetworkRequest request((QUrl(url)));

    mNetworkReply = mNetworkManager->get(request);
    connect(mNetworkReply, SIGNAL(finished()), this, SLOT(slotExtractProfile()));

    // 입력값 clear
    ui->leName->clear();
}

// 응답 결과로부터 Profile(Json형식) 부분 추출
void Profile::slotExtractProfile()
{
    mNetworkReply->disconnect();

    const QString profileStart = "$.Profile = ";
    const QString profileEnd = "};";

    QString responseData = mNetworkReply->readAll();
    qsizetype profileIndex = responseData.indexOf(profileStart);
    qsizetype profileSize = 0;

    // 존재하지 않는 캐릭터인 경우 알람 popup 후 종료
    if (profileIndex == -1)
    {
        QMessageBox msgBox;
        msgBox.setText("존재하지 않는 캐릭터입니다.");
        msgBox.exec();
        return;
    }
    else
    {
        // Profile의 시작 index와 Profile의 크기 계산
        profileIndex += profileStart.size();
        profileSize = responseData.indexOf(profileEnd) - profileIndex + 1;

        // Profile 추출 후 QJSonObject로 변환
        const QString& profile = responseData.sliced(profileIndex, profileSize);
        if (mProfile != nullptr)
            delete mProfile;
        mProfile = new QJsonObject(QJsonDocument::fromJson(profile.toUtf8()).object());
    }
}



















