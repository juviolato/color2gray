#include "dialog.h"

Dialog::Dialog()
{
    inputTitle = INPUT_BOX_TITLE;
    inputPrompt = INPUT_BOX_PROMPT;
    errorMessage = READ_ERROR_MESSAGE;
}

QString Dialog::readUserInput()
{
    QInputDialog *popup = new QInputDialog(nullptr, nullptr);
    QString input;
    bool read;

    do
    {
        input = popup->getText(nullptr, this->inputTitle, this->inputPrompt, QLineEdit::Normal, "", &read);
        if (read && input.isEmpty())
            QMessageBox::information(nullptr, "ERROR", this->errorMessage);

    } while (read && input.isEmpty());

    if (read)
        return input;
    else
        return nullptr;
}

void Dialog::showMessage(QString message)
{
    QMessageBox *popup = new QMessageBox();
    popup->setWindowTitle("Message");
    popup->setText(message);
    popup->setButtonText(0, "Ok");
    popup->show();
}
