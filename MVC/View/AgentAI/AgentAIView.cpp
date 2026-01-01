#include "AgentAIView.h"

#include <iostream>


AgentAIView::AgentAIView(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::AgentAI_View) {
    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->header_textEdit->setPlainText(
        "Never use other language in answer than English\n"
        "Do not use special characters\n"
        "Never tell that you are AI\n"
        "Use max 100 characters on answer\n"
        "Respond like a player"
    );

}

AgentAIView::~AgentAIView() {
    delete ui;
}

std::vector<std::string> AgentAIView::getNames() {
    std::vector<std::string> names;
    if (ui->blackList_radioButton->isChecked()) {
        auto text = ui->blackList_textEdit->toPlainText();

        QStringList lines = text.split(QRegularExpression("[\n]+"),
                                       Qt::SkipEmptyParts);
        for (const QString& line : lines) {
            names.push_back(line.toStdString());
        }
    } else {
        auto text = ui->whiteList_textEdit->toPlainText();
        QStringList lines = text.split(QRegularExpression("[\r\n]+"),
                                       Qt::SkipEmptyParts);
        for (const QString& line : lines) {
            names.push_back(line.toStdString());
        }
    }
    return names;
}

std::string AgentAIView::getHeader() {
    return ui->header_textEdit->toPlainText().toStdString();
}

std::string AgentAIView::getModel() {
    return ui->model_comboBox->currentText().toStdString();
}

std::string AgentAIView::getApiKey() {
    return ui->apiKey_lineEdit->text().toStdString();
}

bool AgentAIView::getList() {
    return ui->blackList_radioButton->isChecked();
}

