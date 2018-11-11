#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>

class Dialog
{
private:
    const QString INPUT_BOX_TITLE = "Enter input";
    const QString INPUT_BOX_PROMPT = "Your input: ";
    const QString READ_ERROR_MESSAGE = "Input could not be read.";

    QString inputTitle;
    QString inputPrompt;
    QString errorMessage;

public:
    QString readUserInput();
    void showMessage(QString message);

    Dialog();
    ~Dialog() {}
};

#endif // DIALOG_H
