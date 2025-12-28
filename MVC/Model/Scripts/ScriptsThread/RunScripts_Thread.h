#ifndef SCRIPTSTHREAD_H
#define SCRIPTSTHREAD_H
#include <QThread>
#include <lua.hpp>
#include "../../const.h"
#include "../../proto_functions_client.h"

class RunScripts_Thread : public QThread {
    Q_OBJECT
    public:
    explicit RunScripts_Thread(const std::vector<Script>& scripts, QObject *parent = nullptr)
        : QThread(parent), m_scripts(scripts), L(nullptr) {}

protected:
    void run() override;

private:
    void initLua();
    void closeLua();
    void executeLuaScript(const std::string& scriptText);

    lua_State* L;
    std::vector<Script> m_scripts;
};

#endif // SCRIPTSTHREAD_H
