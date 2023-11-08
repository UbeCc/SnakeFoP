#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QDialog>
#include "playpage.h"

namespace Ui
{
    class ResultPage;
}

class PlayPage;

class ResultPage : public QDialog
{
Q_OBJECT

public:
    explicit ResultPage(QWidget *parent = nullptr);

    void showEvent(QShowEvent *) override;

    ~ResultPage() override;

private slots:

    void OnReplayButtonClicked();

private:
    PlayPage *playPage;
    Ui::ResultPage *ui;
};

#endif // RESULTPAGE_H
