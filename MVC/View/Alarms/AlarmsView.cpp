//
// Created by Wojciech on 08.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AlarmsView.h" resolved

#include "AlarmsView.h"
#include "ui_AlarmsView.h"


AlarmsView::AlarmsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Alarms_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
}

AlarmsView::~AlarmsView() {
    delete ui;
}
