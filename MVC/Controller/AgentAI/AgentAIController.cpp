#include "AgentAIController.h"

#include <iostream>
#include <QJsonArray>


AgentAIController::AgentAIController(QObject *parent)
    : QObject(parent) {
    m_model = new AgentAIModel(this);
    m_view = new AgentAIView();
    m_view->show();

}

AgentAIController::~AgentAIController() {
}

void AgentAIController::showView() {
    m_view->showNormal();
    m_view->show();
}


void AgentAIController::startAgent_slot(bool state) {
    auto names = m_view->getNames();
    auto header = m_view->getHeader();
    auto model = m_view->getModel();
    auto apiKey = m_view->getApiKey();
    auto list = m_view->getList();
    m_model->startAgent(state, names, header, model, apiKey, list);
}

