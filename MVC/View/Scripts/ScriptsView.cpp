#include "ScriptsView.h"

#include <QCheckBox>
#include <QTextEdit>

#include "Highlighter.h"
#include "ui_ScriptsView.h"


ScriptsView::ScriptsView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Scripts_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    ui->scripts_tableWidget->horizontalHeader()
        ->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->scripts_tableWidget->horizontalHeader()
        ->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->rowCount();
        ui->scripts_tableWidget->insertRow(row);

        auto *textEdit = new QTextEdit(ui->scripts_tableWidget);
        ui->scripts_tableWidget->setCellWidget(row, 0, textEdit);
        new Highlighter(textEdit->document());

        auto *checkBox = new QCheckBox();

        QWidget *checkBoxWidget = new QWidget(ui->scripts_tableWidget);
        QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
        layout->addWidget(checkBox);
        layout->setAlignment(Qt::AlignCenter);
        layout->setContentsMargins(0, 0, 0, 0);
        ui->scripts_tableWidget->setCellWidget(row, 1, checkBoxWidget);
    });
}

ScriptsView::~ScriptsView() {
    delete ui;
}
