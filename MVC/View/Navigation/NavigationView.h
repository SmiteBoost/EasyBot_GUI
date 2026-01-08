//
// Created by Wojciech on 08.01.2026.
//

#ifndef NAVIGATIONVIEW_H
#define NAVIGATIONVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Navigation_View; }
QT_END_NAMESPACE

class NavigationView : public QMainWindow {
Q_OBJECT

public:
    explicit NavigationView(QWidget *parent = nullptr);
    ~NavigationView() override;

private:
    Ui::Navigation_View *ui;
};


#endif //NAVIGATIONVIEW_H
