#include "ScriptEditorDialog.h"
#include "Highlighter.h"

ScriptEditorDialog::ScriptEditorDialog(const QString &script, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Script Editor");
    resize(400, 400);

    auto *layout = new QVBoxLayout(this);

    m_textEdit = new QTextEdit(this);
    m_textEdit->setPlainText(script);
    new Highlighter(m_textEdit->document());

    layout->addWidget(m_textEdit);

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

QString ScriptEditorDialog::getScript() const {
    return m_textEdit->toPlainText();
}
