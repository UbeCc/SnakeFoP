#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileInfo>
#include "recordmanager.h"
#include "playpage.h"
#include "settingpage.h"
#include "replaypage.h"
#include "mapeditor.h"
#include "configeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainPage;
}
QT_END_NAMESPACE

const int TIME_INTERVAL = 1000;

class PlayPage;

class ReplayPage;

class SettingPage;

class Widget : public QWidget
{
Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget() override;

    void ResetRecord(const Map &, const Config &);

    void ResetRecord(const Record &);

    void ResetRecord();

    void UpdateRecordFood(int, int, int);

    void UpdateRecordMovement(char);

    void UpdateTime(int);

    [[maybe_unused]] void PrintRecord();

    pair<Point, int> GetNextFood();

    char GetNextMovement();

    bool GetMode() const;

    Record &GetGameRecord();

    char NextAction();

    char GetCurrentAction();

    int GetCurrentStep();

    [[nodiscard]] bool IsEnd() const;

    bool mode;

    [[nodiscard]] Record GetRecord() const;

    [[nodiscard]] Map GetMap() const;

    [[nodiscard]] const Config &GetConfig() const;

    void SetGameConfigPath(const QFileInfo &path);

    void SetGameMapPath(const QFileInfo &path);

    void SetMode(bool _mode);

    [[nodiscard]] QFileInfo GetGameConfigPath() const;

    [[nodiscard]] QFileInfo GetGameMapPath() const;

private slots:

    void OnEnterButtonClicked();

    void OnReplayButtonClicked();

    void OnMapEditorButtonClicked();

    void OnConfigEditorButtonClicked();

private:
    Ui::MainPage *ui;
    SettingPage *settingPage;
    ReplayPage *replayPage;
    PlayPage *playPage;
    QFileInfo gameConfigPath, gameMapPath;
    MapEditor *mapEditor;
    ConfigEditor *configEditor;
public:
    int seqPtr;
    int movementPtr;
    int foodPtr;
    Record gameRecord;
};

#endif // WIDGET_H
