#include "MiscellaneousView.h"
#include "ui_MiscellaneousView.h"


MiscellaneousView::MiscellaneousView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Miscellaneous_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

MiscellaneousView::~MiscellaneousView() {
    delete ui;
}
