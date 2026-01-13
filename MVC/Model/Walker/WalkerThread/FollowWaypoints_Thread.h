#ifndef FOLLOWWAYPOINTS_THREAD_H
#define FOLLOWWAYPOINTS_THREAD_H
#include <QThread>
#include <string>
#include "../../const.h"
#include "../../proto_functions_client.h"

class FollowWaypoints_Thread : public QThread {
    Q_OBJECT
    public:
    explicit FollowWaypoints_Thread(const std::vector<Waypoint> &waypoints, QObject *parent = nullptr)
        : QThread(parent), waypoints(waypoints) {}
    
protected:
    void run() override;
    signals:
    void indexUpdate_signal(int index);
private:
    std::vector<Waypoint> waypoints;
    size_t index = 0;


    void findClosestWaypoint();
    void findNextValidWaypoint();
    void performWalk(Waypoint wpt, uintptr_t localPlayer, Position playerPos);
    void performAction(Waypoint wpt);
    void performUse(Waypoint wpt, uintptr_t localPlayer, Position playerPos);
    Otc::Direction getDirection(const std::string& wpt_direction);
};



#endif //FOLLOWWAYPOINTS_THREAD_H
