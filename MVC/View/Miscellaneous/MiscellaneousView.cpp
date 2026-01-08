//
// Created by Wojciech on 08.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MiscellaneousView.h" resolved

#include "MiscellaneousView.h"
#include "ui_MiscellaneousView.h"


MiscellaneousView::MiscellaneousView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Miscellaneous_View) {
    ui->setupUi(this);
}

MiscellaneousView::~MiscellaneousView() {
    delete ui;
}
