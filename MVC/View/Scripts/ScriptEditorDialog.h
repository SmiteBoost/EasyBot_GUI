#ifndef SCRIPTEDITORDIALOG_H
#define SCRIPTEDITORDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>

class ScriptEditorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ScriptEditorDialog(const QString &script, QWidget *parent = nullptr);

    QString getScript() const;

private:
    QTextEdit *m_textEdit;
};

#endif // SCRIPTEDITORDIALOG_H
