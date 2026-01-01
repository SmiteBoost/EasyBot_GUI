#ifndef AGENTTHREAD_H
#define AGENTTHREAD_H
#include <QThread>
#include "../../const.h"


class AgentThread : public QThread {
    Q_OBJECT
public:
    explicit AgentThread(const std::vector<std::string> &names, std::string header, std::string model, std::string apiKey, bool list,  QObject *parent = nullptr)
    : QThread(parent), names(names), header(header), model(model), apiKey(apiKey), list(list) {}
protected:
    void run() override;
private:
    std::vector<std::string> names;
    std::string model;
    std::string header;
    std::string apiKey;
    bool list;

    std::string performRequest_GPT(MessageStruct message);


};



#endif //AGENTTHREAD_H
