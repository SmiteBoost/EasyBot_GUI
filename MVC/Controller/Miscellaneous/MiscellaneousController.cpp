#include "MiscellaneousController.h"
#include <iostream>
#include <QJsonArray>


MiscellaneousController::MiscellaneousController(QObject *parent)
    : QObject(parent) {
    m_model = new MiscellaneousModel(this);
    m_view = new MiscellaneousView();
    m_view->show();

}

MiscellaneousController::~MiscellaneousController() {
}

void MiscellaneousController::showView() {
    m_view->showNormal();
    m_view->show();
}


