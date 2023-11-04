#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include "playpage.h"
#include "settingpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

class PlayPage;
class SettingPage;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void SetGameConfigPath(const QFileInfo &path);
    void SetGameMapPath(const QFileInfo &path);
    [[nodiscard]] QFileInfo GetGameConfigPath() const;
    [[nodiscard]] QFileInfo GetGameMapPath() const;

private slots:
    void on_enterButton_clicked();

private:
    Ui::MainPage *ui;
    SettingPage *settingPage;
    PlayPage *playPage;
    QFileInfo gameConfigPath, gameMapPath;
};
#endif // WIDGET_H
