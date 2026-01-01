#include "AgentAIModel.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


AgentAIModel::AgentAIModel(QObject *parent)
    : QObject(parent) {

}

AgentAIModel::~AgentAIModel() {
    if (startAgentThread) {
        startAgentThread->quit();
        startAgentThread->wait();
        delete startAgentThread;
    }
}

void AgentAIModel::startAgent(bool state, const std::vector<std::string> &names, std::string header, std::string model, std::string apiKey, bool list) {
    if (state) {
        if (!startAgentThread) {
            startAgentThread = new AgentThread(names, header, model, apiKey, list, this);
            connect(startAgentThread, &QThread::finished, startAgentThread, &QObject::deleteLater);
            connect(startAgentThread, &QThread::finished, this, [this]() {
                this->startAgentThread = nullptr;
            });
            startAgentThread->start();
        }
    } else {
        if (startAgentThread) {
            startAgentThread->requestInterruption();
            startAgentThread->quit();
        }
    }
}


