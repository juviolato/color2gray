#include <QApplication>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "dialog.h"
#include "color2gray.h"
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog *popup = new Dialog();
    popup->setInputTitle("Open image");
    popup->setInputPrompt("Image file name:");
    QString fileName = popup->readUserInput();
    Mat originalColorImage = imread(fileName.toUtf8().constData());

    Mat image;
    image = imread(fileName.toUtf8().constData());

    ParameterHolder *parameters = new ParameterHolder();
    double theta, alpha;

    popup->setInputTitle("Parameter definition");
    popup->setInputPrompt("Theta value in degrees (default is 45º):");
    theta = popup->readUserInput().toDouble();
    theta *= PI/180;        // Converte theta para radianos

    popup->setInputPrompt("Alpha value (default is 10):");
    alpha = popup->readUserInput().toDouble();

    parameters->theta = theta;
    parameters->alpha = alpha;

    image = color2gray(image, *parameters);
    imwrite("out.jpg", image);

    return 0;
}
