#include <QDebug>
#include "playpage.h"
#include "ui_playpage.h"

PlayPage::PlayPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayPage)
{
    ui->setupUi(this);
}

PlayPage::~PlayPage()
{
    delete ui;
}

void PlayPage::on_btn_clicked() {
    gameOver();
}

void PlayPage::gameOver() {
    qDebug() << "test" << Qt::endl;
    this->done(0);
    resultPage->show();
}

void PlayPage::initPlay() {

}
