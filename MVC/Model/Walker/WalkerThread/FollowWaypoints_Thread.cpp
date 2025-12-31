#include "FollowWaypoints_Thread.h"
#include <QElapsedTimer>
#include <iostream>
#include "../../BotEngine.h"
#include "../../../../LuaEngine.h"

void FollowWaypoints_Thread::run() {
    if (waypoints.empty()) return;
    
    // Start the Lua script if one is set
    if (!luaScriptText.empty()) {
        luaScriptEngine = new LuaEngine(luaScriptText, nullptr);
        luaScriptEngine->start();
        std::cout << "Started Lua script for walker" << std::endl;
    }
    
    size_t index = findClosest();
    while (!isInterruptionRequested()) {
        if (engine->hasTarget && (index + 1) % waypoints.size() != 0) {
            index += bestWpt(waypoints[index], waypoints[index + 1]);
        }
        auto localPlayer = proto->getLocalPlayer();
        auto wpt = waypoints[index];
        auto playerPos = proto->getPosition(localPlayer);
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            index = (index + 1) % waypoints.size();
            if (wpt.option == "Action") {
                auto* actionEngine = new LuaEngine(wpt.action, nullptr);
                actionEngine->start();
                while (actionEngine->isRunning()) {
                    if (isInterruptionRequested()) {
                        actionEngine->requestStop();
                        actionEngine->wait(1000);
                        delete actionEngine;
                        return;
                    }
                    msleep(100);
                }
                std::string returnedLabel = actionEngine->getReturnedString();
                delete actionEngine;
                if (!returnedLabel.empty()) {
                    for (size_t i = 0; i < waypoints.size(); ++i) {
                        if (waypoints[i].action == returnedLabel) {
                            index = i;
                        }
                    }
                }
            }
            emit indexUpdate_signal(static_cast<int>(index));
            continue;
        }
        // Only walks if we dont have a target or we want to Lure
        if (!engine->hasTarget || wpt.option == "Lure") {
            if (!proto->isAutoWalking(localPlayer)) {
                performWalk(wpt, localPlayer);
            }
        }
        msleep(50);
    }
    
    // Stop the Lua script if running
    if (luaScriptEngine) {
        std::cout << "Stopping walker Lua script..." << std::endl;
        luaScriptEngine->requestStop();
        
        if (!luaScriptEngine->wait(1000)) { // 1 second timeout
            std::cerr << "Warning: Walker Lua script did not finish in time, terminating forcefully" << std::endl;
            luaScriptEngine->terminate();
            luaScriptEngine->wait();
        }
        
        delete luaScriptEngine;
        luaScriptEngine = nullptr;
        std::cout << "Walker Lua script stopped" << std::endl;
    }
}

void FollowWaypoints_Thread::performWalk(Waypoint wpt, uintptr_t localPlayer) {
    if (wpt.direction != "C") {
        auto direction = getDirection(wpt.direction);
        proto->walk(direction);
        return;
    }
    auto playerPos = proto->getPosition(localPlayer);
    if ((playerPos.x != wpt.position.x || playerPos.y != wpt.position.y) && playerPos.z == wpt.position.z) {
        proto->autoWalk(localPlayer, wpt.position, false);
    }
}

Otc::Direction FollowWaypoints_Thread::getDirection(const std::string& wpt_direction) {
    if (wpt_direction == "N") return Otc::North;
    if (wpt_direction == "E") return Otc::East;
    if (wpt_direction == "S") return Otc::South;
    if (wpt_direction == "W") return Otc::West;
    if (wpt_direction == "NW") return Otc::NorthWest;
    if (wpt_direction == "NE") return Otc::NorthEast;
    if (wpt_direction == "SW") return Otc::SouthWest;
    if (wpt_direction == "SE") return Otc::SouthEast;
    return Otc::InvalidDirection;
}

int FollowWaypoints_Thread::findClosest() {
    int closestIndex = 0;
    int minDistance = std::numeric_limits<int>::max();
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    for (int i = 0; i < waypoints.size(); ++i) {
        const auto& wpt = waypoints[i];
        if (playerPos.z != wpt.position.z) {
            continue;
        }
        int dist = std::max(
            std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y))
        );
        if (dist < minDistance) {
            minDistance = dist;
            closestIndex = i;
        }
    }

    return closestIndex;
}


int FollowWaypoints_Thread::bestWpt(Waypoint first_wpt, Waypoint second_wpt) {
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);

    auto f = first_wpt.position;
    auto s = second_wpt.position;
    if (f.z != s.z)
        return 0;

    float distFirst =
        (f.x - playerPos.x) * (f.x - playerPos.x) +
        (f.y - playerPos.y) * (f.y - playerPos.y);

    float distSecond =
        (s.x - playerPos.x) * (s.x - playerPos.x) +
        (s.y - playerPos.y) * (s.y - playerPos.y);

    return (distFirst < distSecond) ? 0 : 1;
}

void FollowWaypoints_Thread::setLuaScript(const std::string& script) {
    luaScriptText = script;
}

void FollowWaypoints_Thread::clearLuaScript() {
    luaScriptText.clear();
}

