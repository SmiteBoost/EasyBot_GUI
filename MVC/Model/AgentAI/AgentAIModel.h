#ifndef AGENTAIMODEL_H
#define AGENTAIMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"
#include "AgentThread/AgentThread.h"


class AgentAIModel : public QObject {
    Q_OBJECT
public:
    explicit AgentAIModel(QObject *parent = nullptr);
    ~AgentAIModel();

    void startAgent(bool state, const std::vector<std::string> &names, std::string header, std::string model, std::string apiKey, bool list);


private:
    AgentThread *startAgentThread = nullptr;
};



#endif //AGENTAIMODEL_H
