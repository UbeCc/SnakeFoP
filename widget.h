#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include "recordmanager.h"
#include "playpage.h"
#include "settingpage.h"
#include "replaypage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

class PlayPage;
class RePlayPage;
class SettingPage;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    static void ResetRecord(Map, Config);
    static void ResetRecord(Record);
    static void UpdateRecordFood(int, int, int);
    static void UpdateRecordMovement(char);
    static void PrintFood();
    static pair<Point, int> GetNextFood();
    [[nodiscard]] static Record GetRecord();
    [[nodiscard]] static Map GetMap();
    [[nodiscard]] static Config GetConfig();
    void SetGameConfigPath(const QFileInfo &path);
    void SetGameMapPath(const QFileInfo &path);
    [[nodiscard]] QFileInfo GetGameConfigPath() const;
    [[nodiscard]] QFileInfo GetGameMapPath() const;

private slots:
    void on_enterButton_clicked();
    void on_replayButton_clicked();

private:
    Ui::MainPage *ui;
    SettingPage *settingPage;
    RePlayPage *replayPage;
    PlayPage *playPage;
    QFileInfo gameConfigPath, gameMapPath;
public:
    static int foodPtr;
    static Record gameRecord;
};
#endif // WIDGET_H
