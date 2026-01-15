#include "ScriptsConsole.h"
#include "../../../LuaBindings.h"
#include <QString>
#include <QThread>
#include <string>

ScriptsConsole::ScriptsConsole(QObject* parent) : QObject(parent) {}
ScriptsConsole::~ScriptsConsole() { shutdown(); }

void ScriptsConsole::init() {
    if (L) return;
    L = luaL_newstate();
    luaL_openlibs(L);

    // store this in registry for callbacks
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, "ScriptsConsole_instance");

    registerBindings();
    globalizeProto();
    installPrint();
}

void ScriptsConsole::shutdown() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void ScriptsConsole::registerBindings() {
    LuaBindings::registerAll(L);
}

void ScriptsConsole::globalizeProto() {
    const char* snippet =
        "setmetatable(_G, {"
        "  __index = function(_, key)"
        "    local val = proto[key]"
        "    if type(val) == 'function' then"
        "      return function(...) return val(proto, ...) end"
        "    end"
        "    return val"
        "  end"
        "})";
    if (luaL_dostring(L, snippet) != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        emit output(QString("Globalize Error: ") + err);
        lua_pop(L, 1);
    }
}

int ScriptsConsole::luaPrintRedirect(lua_State* Ls) {
    // get instance
    lua_getfield(Ls, LUA_REGISTRYINDEX, "ScriptsConsole_instance");
    auto* self = static_cast<ScriptsConsole*>(lua_touserdata(Ls, -1));
    lua_pop(Ls, 1);

    int n = lua_gettop(Ls);
    std::string msg;
    for (int i = 1; i <= n; ++i) {
        const char* s = luaL_tolstring(Ls, i, nullptr);
        if (s) { msg += s; }
        if (i < n) msg += "\t";
        lua_pop(Ls, 1); // pop result of tolstring
    }
    if (self) self->output(QString::fromStdString(msg));
    return 0;
}

void ScriptsConsole::installPrint() {
    lua_pushcfunction(L, &ScriptsConsole::luaPrintRedirect);
    lua_setglobal(L, "print");
}

void ScriptsConsole::emitStackResults(int baseTop) {
    int top = lua_gettop(L);
    int nres = top - baseTop;
    if (nres <= 0) return;
    // concatenate results into a single line
    QString line;
    for (int i = baseTop + 1; i <= top; ++i) {
        const char* s = luaL_tolstring(L, i, nullptr);
        if (s) { line += QString::fromUtf8(s); }
        if (i < top) line += "\t";
        lua_pop(L, 1);
    }
    if (!line.isEmpty()) emit output(line);
    // clear stack
    lua_settop(L, baseTop);
}

void ScriptsConsole::execute(const QString& code) {
    if (!L) init();
    const std::string src = code.toStdString();

    // Try as expression: wrap with return ...
    int base = lua_gettop(L);
    std::string expr = std::string("return ") + src;
    int status = luaL_loadstring(L, expr.c_str());
    if (status == LUA_OK) {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (status == LUA_OK) {
            emitStackResults(base);
            return;
        } else {
            const char* err = lua_tostring(L, -1);
            emit output(QString("Error: ") + err);
            lua_pop(L, 1);
            // fall through to statement execution
        }
    }

    // Execute as statement
    base = lua_gettop(L);
    status = luaL_loadstring(L, src.c_str());
    if (status != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        emit output(QString("Syntax Error: ") + err);
        lua_pop(L, 1);
        return;
    }
    status = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (status != LUA_OK) {
        const char* err = lua_tostring(L, -1);
        emit output(QString("Runtime Error: ") + err);
        lua_pop(L, 1);
        return;
    }
    emitStackResults(base);
}
