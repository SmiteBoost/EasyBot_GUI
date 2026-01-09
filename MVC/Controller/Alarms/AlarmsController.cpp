#include "AlarmsController.h"
#include <iostream>
#include <QJsonArray>


AlarmsController::AlarmsController(QObject *parent)
    : QObject(parent) {
    m_model = new AlarmsModel(this);
    m_view = new AlarmsView();
    m_view->show();

}

AlarmsController::~AlarmsController() {
}

void AlarmsController::showView() {
    m_view->showNormal();
    m_view->show();
}


