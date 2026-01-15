#include "ScriptsModel.h"

#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


ScriptsModel::ScriptsModel(QObject *parent)
    : QObject(parent) {
}

ScriptsModel::~ScriptsModel() {
    if (runScriptsThread) {
        runScriptsThread->quit();
        runScriptsThread->wait();
        delete runScriptsThread;
    }
}

void ScriptsModel::addItem(const int &id, bool state, const QString &name, const QString &script_text, int sleepTime) {
    Script script;
    script.name = name.toStdString();
    script.text = script_text.toStdString();
    script.enabled = state;
    script.sleepTime = sleepTime;
    if (id < scripts.size()) {
        scripts[id] = script;
    } else {
        while (scripts.size() < id) {
            scripts.push_back(Script{});
        }
        scripts.push_back(script);
    }
}

void ScriptsModel::deleteItem(const int& index) {
    if (index >= 0 && index < scripts.size()) {
        scripts.erase(scripts.begin() + index);
    }
}

void ScriptsModel::startScripts(bool state) {
    if (state) {
        if (!runScriptsThread) {
            runScriptsThread = new RunScripts_Thread(scripts, this);
            connect(runScriptsThread, &QThread::finished, runScriptsThread, &QObject::deleteLater);
            connect(runScriptsThread, &QThread::finished, this, [this]() {
                this->runScriptsThread = nullptr;
            });
            connect(runScriptsThread, &RunScripts_Thread::consoleOutput, this, &ScriptsModel::consoleOutput_signal);
            runScriptsThread->start();
        }
    } else {
        if (runScriptsThread) {
            runScriptsThread->requestInterruption();
            runScriptsThread->quit();
        }
    }
}


QJsonArray ScriptsModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &script : scripts) {
        QJsonObject obj;
        obj["name"] = script.name.c_str();
        obj["text"] = script.text.c_str();
        obj["enabled"] = script.enabled;
        obj["sleep_time"] = script.sleepTime;
        jsonArray.append(obj);
    }
    return jsonArray;
}

void ScriptsModel::fromJson(const QJsonArray &json) {
    scripts.clear();
    emit clearTableWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        Script script;
        script.name = obj["name"].toString().toStdString();
        script.text = obj["text"].toString().toStdString();
        script.enabled = obj["enabled"].toBool();
        script.sleepTime = obj.contains("sleep_time") ? obj["sleep_time"].toInt() : 100;
        scripts.push_back(script);
        emit addItem_signal(script.enabled, QString::fromStdString(script.name), QString::fromStdString(script.text), script.sleepTime);
    }
}
