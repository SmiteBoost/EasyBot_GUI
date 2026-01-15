#include "ScriptsView.h"

#include <iostream>
#include <QCheckBox>
#include <QTextEdit>
#include <QEvent>
#include "ScriptEditorDialog.h"

#include "Highlighter.h"
#include "ui_ScriptsView.h"


ScriptsView::ScriptsView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Scripts_View) {
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->setupUi(this);


    ui->scripts_tableWidget->setColumnCount(3);
    ui->scripts_tableWidget->setHorizontalHeaderLabels({"Sleep (ms)", "Scripts", "Status"});
    ui->scripts_tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->scripts_tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->scripts_tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    connect(ui->remove_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->currentRow();
        if (row != -1) {
            ui->scripts_tableWidget->removeRow(row);
            emit deleteItem_signal(row);
        }
    });

    connect(ui->add_pushButton, &QPushButton::clicked, this, [this]() {
        int row = ui->scripts_tableWidget->rowCount();
        addItem(false, "New Script", "", 100);
        emit addItem_signal(row, false, "New Script", "", 100);
    });

    connect(ui->scripts_tableWidget, &QTableWidget::itemDoubleClicked, this, [this](QTableWidgetItem *item) {
        if (item->column() == 1 && (item->flags() & Qt::ItemIsEnabled)) {
            QString currentName = item->text();
            QString currentScript = item->data(Qt::UserRole).toString();

            ScriptEditorDialog dialog(currentName, currentScript, this);
            if (dialog.exec() == QDialog::Accepted) {
                item->setText(dialog.getName());
                item->setData(Qt::UserRole, dialog.getScript());

                int row = item->row();
                QWidget *cellWidget = ui->scripts_tableWidget->cellWidget(row, 2);
                if (cellWidget) {
                    QCheckBox *checkBox = cellWidget->findChild<QCheckBox *>();
                    bool isChecked = checkBox ? checkBox->isChecked() : false;

                    QWidget *sleepWidget = ui->scripts_tableWidget->cellWidget(row, 0);
                    int sleepVal = 100;
                    if (sleepWidget) {
                        QLineEdit *sleepEdit = sleepWidget->findChild<QLineEdit *>();
                        if (sleepEdit) sleepVal = sleepEdit->text().toInt();
                    }

                    emit addItem_signal(row, isChecked, dialog.getName(), dialog.getScript(), sleepVal);
                }
            }
        }
    });

    // Console input: Enter to execute
    connect(ui->consoleInput_lineEdit, &QLineEdit::returnPressed, this, [this]() {
        const QString code = ui->consoleInput_lineEdit->text();
        if (!code.isEmpty()) {
            emit executeConsole_signal(code);
            ui->consoleInput_lineEdit->clear();
        }
    });
    connect(ui->consoleRun_pushButton, &QPushButton::clicked, this, [this]() {
        const QString code = ui->consoleInput_lineEdit->text();
        if (!code.isEmpty()) {
            emit executeConsole_signal(code);
            ui->consoleInput_lineEdit->clear();
        }
    });
    connect(ui->consoleClear_pushButton, &QPushButton::clicked, this, [this]() {
        ui->consoleOutput_plainTextEdit->clear();
    });
}

void ScriptsView::addItem(bool state, const QString &name, const QString &script_text, int sleepTime) {
    int row = ui->scripts_tableWidget->rowCount();
    ui->scripts_tableWidget->insertRow(row);

    auto *sleepLineEdit = new QLineEdit();
    sleepLineEdit->setFixedWidth(50);
    sleepLineEdit->setText(QString::number(sleepTime));
    sleepLineEdit->setValidator(new QIntValidator(0, 1000000, this));

    auto *sleepWidget = new QWidget(ui->scripts_tableWidget);
    auto *sleepLayout = new QHBoxLayout(sleepWidget);
    sleepLayout->addWidget(sleepLineEdit);
    sleepLayout->setAlignment(Qt::AlignCenter);
    sleepLayout->setContentsMargins(0, 0, 0, 0);
    ui->scripts_tableWidget->setCellWidget(row, 0, sleepWidget);

    auto *nameItem = new QTableWidgetItem(name);
    nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
    nameItem->setData(Qt::UserRole, script_text);
    ui->scripts_tableWidget->setItem(row, 1, nameItem);

    auto *checkBox = new QCheckBox();
    checkBox->setChecked(state);

    auto updateSignal = [this, checkBox, sleepLineEdit, nameItem]() {
        int currentRow = -1;
        for (int i = 0; i < ui->scripts_tableWidget->rowCount(); ++i) {
            if (ui->scripts_tableWidget->item(i, 1) == nameItem) {
                currentRow = i;
                break;
            }
        }
        if (currentRow == -1) return;

        bool checked = checkBox->isChecked();
        QString name = nameItem->text();
        QString scriptText = nameItem->data(Qt::UserRole).toString();
        int sleepVal = sleepLineEdit->text().toInt();

        if (checked) {
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEnabled);
            sleepLineEdit->setEnabled(false);
        } else {
            nameItem->setFlags(nameItem->flags() | Qt::ItemIsEnabled);
            sleepLineEdit->setEnabled(true);
        }
        emit addItem_signal(currentRow, checked, name, scriptText, sleepVal);
    };

    connect(checkBox, &QCheckBox::toggled, this, updateSignal);
    connect(sleepLineEdit, &QLineEdit::editingFinished, this, updateSignal);

    QWidget *checkBoxWidget = new QWidget(ui->scripts_tableWidget);
    QHBoxLayout *layout = new QHBoxLayout(checkBoxWidget);
    layout->addWidget(checkBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->scripts_tableWidget->setCellWidget(row, 2, checkBoxWidget);

    // Initial state handling
    if (state) {
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEnabled);
        sleepLineEdit->setEnabled(false);
    }
}

void ScriptsView::clearTableWidget() {
    ui->scripts_tableWidget->setRowCount(0);
}

ScriptsView::~ScriptsView() {
    delete ui;
}

bool ScriptsView::eventFilter(QObject *obj, QEvent *event) {
    return QMainWindow::eventFilter(obj, event);
}

void ScriptsView::appendConsole(const QString &text) {
    if (!text.isEmpty()) {
        ui->consoleOutput_plainTextEdit->appendPlainText(text);
    }
}

