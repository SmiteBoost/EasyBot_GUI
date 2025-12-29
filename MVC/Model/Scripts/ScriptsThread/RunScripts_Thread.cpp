#include "RunScripts_Thread.h"
#include "../../../../LuaBindings.h"
#include <iostream>
#include <regex>
#include <QRegularExpression>

void RunScripts_Thread::run() {
    initLua();

    for (const auto& script : m_scripts) {
        if (script.enabled) {
            executeLuaScript(script.text);
        }
    }

    while (!isInterruptionRequested()) {
        msleep(10);
    }

    closeLua();
}

void RunScripts_Thread::initLua() {
    L = luaL_newstate();
    luaL_openlibs(L);
    LuaBindings::registerAll(L);

    // Globalize proto functions
    const char* globalizeSnippet = 
        "setmetatable(_G, {"
        "  __index = function(_, key)"
        "    local val = proto[key]"
        "    if type(val) == 'function' then"
        "      return function(...) return val(proto, ...) end"
        "    end"
        "    return val"
        "  end"
        "})";
    
    if (luaL_dostring(L, globalizeSnippet) != LUA_OK) {
        std::cerr << "Globalize Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

void RunScripts_Thread::closeLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}

void RunScripts_Thread::executeLuaScript(const std::string& scriptText) {
    if (!L) return;

    if (luaL_dostring(L, scriptText.c_str()) != LUA_OK) {
        std::cerr << "Lua Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}
