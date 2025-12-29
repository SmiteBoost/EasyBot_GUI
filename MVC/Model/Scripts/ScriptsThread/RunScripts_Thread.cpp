#include "RunScripts_Thread.h"
#include "../../../../LuaBindings.h"
#include "../../BotEngine.h"
#include <iostream>
#include <regex>
#include <QRegularExpression>

void RunScripts_Thread::run() {

    for (const auto& script : m_scripts) {
        if (script.enabled) {
            engine->executeLuaScript(script.text);
        }
    }

    while (!isInterruptionRequested()) {
        msleep(10);
    }
}

