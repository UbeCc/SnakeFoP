#ifndef CONFIGEDITOR_H
#define CONFIGEDITOR_H

#include <QDialog>
#include "game.h"

namespace Ui
{
class ConfigEditor;
}

class ConfigEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigEditor(QWidget *parent = nullptr);
    ~ConfigEditor() override;

private slots:
    void OnNewConfigButtonClicked();

    void OnLoadConfigButtonClicked();

    void OnSaveConfigButtonClicked();

    void OnLevelButtonClicked();

    void OnSeedButtonClicked();

    void OnFoodNumButtonClicked();

    void OnFoodPossibilityButtonClicked();


private:
    Ui::ConfigEditor *ui;
    Config config;
    Game *game;
    const static Map dummyMap;
};

#endif // CONFIGEDITOR_H
