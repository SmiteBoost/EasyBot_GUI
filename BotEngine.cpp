#include "BotEngine.h"
BotEngine* BotEngine::instance{nullptr};
std::mutex BotEngine::mutex;


BotEngine::BotEngine() = default;

BotEngine* BotEngine::getInstance()
{
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr)
    {
        instance = new BotEngine();
        instance->initLua();
    }
    return instance;
}

void BotEngine::executeLuaScript(const std::string &scriptText) {
    if (!L) return;

    if (luaL_dostring(L, scriptText.c_str()) != LUA_OK) {
        std::cerr << "Lua Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

void BotEngine::initLua() {
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

void BotEngine::closeLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
}
