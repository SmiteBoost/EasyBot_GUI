#ifndef MiscellaneousMODEL_H
#define MiscellaneousMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"


class MiscellaneousModel : public QObject {
    Q_OBJECT
public:
    explicit MiscellaneousModel(QObject *parent = nullptr);
    ~MiscellaneousModel();
};



#endif
