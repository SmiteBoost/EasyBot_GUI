#ifndef ALARMSMODEL_H
#define ALARMSMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"


class AlarmsModel : public QObject {
    Q_OBJECT
public:
    explicit AlarmsModel(QObject *parent = nullptr);
    ~AlarmsModel();
};



#endif // ALARMSMODEL_H
