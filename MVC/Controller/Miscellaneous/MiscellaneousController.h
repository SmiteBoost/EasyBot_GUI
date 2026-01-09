#ifndef MiscellaneousCONTROLLER_H
#define MiscellaneousCONTROLLER_H
#include <QObject>
#include "Miscellaneous/MiscellaneousView.h"
#include "Miscellaneous/MiscellaneousModel.h"


class MiscellaneousController : public QObject {
    Q_OBJECT
public:
    explicit MiscellaneousController(QObject *parent = nullptr);
    ~MiscellaneousController();

    void showView();

private:
    MiscellaneousView *m_view;
    MiscellaneousModel *m_model;
};


#endif
