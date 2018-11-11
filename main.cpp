#include "widget.h"
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
    QString fileName = popup->readUserInput();
    Mat originalColorImage = imread(fileName.toUtf8().constData(), 1);
    ParameterHolder *parameters = new ParameterHolder();

    /* testando a rgb2cielab aqui */
    imshow("HARU", originalColorImage);
    originalColorImage = rgb2cielab(originalColorImage);
    imwrite("lab.jpg", originalColorImage);

    color2gray(originalColorImage, *parameters);

    return a.exec();
}
