#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>

namespace Ui {
class Profile;
}

class Profile : public QWidget
{
    Q_OBJECT

public:
    explicit Profile(QWidget *parent = nullptr);
    ~Profile();

private:
    void initUI();
    void initConnect();

private:
    Ui::Profile *ui;
    class QNetworkAccessManager* mNetworkManager;
    class QNetworkReply* mNetworkReply;
    class QJsonObject* mProfile = nullptr;

private slots:
    void slotHome();
    void slotProfileRequest();
    void slotExtractProfile();
};

#endif // PROFILE_H
