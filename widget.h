#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include "recordmanager.h"
#include "playpage.h"
#include "settingpage.h"
#include "replaypage.h"
#include "mapeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainPage; }
QT_END_NAMESPACE

const int TIME_INTERVAL = 2000;

class PlayPage;
class RePlayPage;
class SettingPage;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    static void ResetRecord(const Map&, const Config&);
    static void ResetRecord(const Record&);
    static void UpdateRecordFood(int, int, int);
    static void UpdateRecordMovement(char);
    static void UpdateTime(int);
    static void PrintRecord();
    static pair<Point, int> GetNextFood();
    static char GetNextMovement();
    static char NextAction();
    static char GetCurrentAction();
    static int GetCurrentStep();
    static bool IsEnd();
    static bool mode;
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
    void on_MapEditorButton_clicked();

private:
    Ui::MainPage *ui;
    SettingPage *settingPage;
    RePlayPage *replayPage;
    PlayPage *playPage;
    QFileInfo gameConfigPath, gameMapPath;
    MapEditor *mapEditor;
public:
    static int seqPtr;
    static int movementPtr;
    static int foodPtr;
    static Record gameRecord;
};
#endif // WIDGET_H
