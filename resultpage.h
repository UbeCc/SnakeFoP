#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include <QDialog>

namespace Ui {
class ResultPage;
}

class ResultPage : public QDialog
{
    Q_OBJECT

public:
    explicit ResultPage(QWidget *parent = nullptr);
    virtual void showEvent(QShowEvent*);
    ~ResultPage();

private slots:
    void on_replayButton_clicked();

private:
    Ui::ResultPage *ui;
};

#endif // RESULTPAGE_H
