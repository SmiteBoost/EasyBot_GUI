#ifndef BOTENGINE_H
#define BOTENGINE_H
#include <mutex>
#include <lua.hpp>
#include "../../const.h"
#include "../../proto_functions_client.h"
#include "../../../../LuaBindings.h"
#include <iostream>
#include <regex>
#include <QRegularExpression>

#define engine BotEngine::getInstance()

class BotEngine {
private:
    static BotEngine* instance;
    static std::mutex mutex;
protected:
    BotEngine();
    ~BotEngine(){}
public:
    BotEngine(BotEngine const&) = delete;
    void operator=(const BotEngine&) = delete;
    static BotEngine* getInstance();

    // Engine variables
    bool hasTarget = false;
    bool isLooting = false;
    bool isWalking = false;

    void executeLuaScript(const std::string& scriptText);
private:
    void initLua();
    void closeLua();
    lua_State* L = nullptr;
};



#endif //BOTENGINE_H
