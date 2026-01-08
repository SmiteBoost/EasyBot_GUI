//
// Created by Wojciech on 08.01.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_NavigationView.h" resolved

#include "NavigationView.h"
#include "ui_NavigationView.h"


NavigationView::NavigationView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Navigation_View) {
    ui->setupUi(this);
}

NavigationView::~NavigationView() {
    delete ui;
}
