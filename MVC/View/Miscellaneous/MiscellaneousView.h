//
// Created by Wojciech on 08.01.2026.
//

#ifndef MISCELLANEOUSVIEW_H
#define MISCELLANEOUSVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Miscellaneous_View; }
QT_END_NAMESPACE

class MiscellaneousView : public QMainWindow {
Q_OBJECT

public:
    explicit MiscellaneousView(QWidget *parent = nullptr);
    ~MiscellaneousView() override;

private:
    Ui::Miscellaneous_View *ui;
};


#endif //MISCELLANEOUSVIEW_H
