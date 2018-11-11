#include "color2gray.h"

Mat rgb2cielab(Mat original)
{
    Mat cielab;
    cvtColor(original, cielab, COLOR_BGR2Lab);

    return cielab;
}

double crunch(double value, double alpha)
{
    return alpha * tanh(value / alpha);
}

double dotProduct2(Vec2b v, Vec2b u)
{
    return v[0]*u[0] + v[1]*u[1];
}

double findDelta(double deltaL, Vec2b deltaAB, double deltaABNorm, double theta, double alpha)
{
    Vec2b vtheta = Vec2b(static_cast<uchar>(cos(theta)), static_cast<uchar>(sin(theta)));
    double delta;

    if (fabs(deltaL) > crunch(deltaABNorm, alpha))
        delta = deltaL;
    else if (dotProduct2(deltaAB, vtheta) >=0)
        delta = crunch(deltaABNorm, alpha);
    else
        delta = crunch(-deltaABNorm, alpha);

    return delta;
}

Mat color2gray(Mat original, ParameterHolder parameters)
{
    int width{original.cols}, height{original.rows};

    vector<double> pixel1ColumnPosition(static_cast<ulong>(height), 0);
    vector< vector<double> > pixel1RowPosition(static_cast<ulong>(width), pixel1ColumnPosition);
    vector< vector< vector<double> > > pixel2ColumnPosition(static_cast<ulong>(height), pixel1RowPosition);
    vector< vector< vector< vector<double> > > > delta(static_cast<ulong>(width), pixel2ColumnPosition);

    // Passo 1: converter a imagem para o espaço CIE L*a*b*
    Mat cielabImg = rgb2cielab(original);
    Mat gray;

    // Passo 2: encontrar as target differences
    Vec3b labPixel1, labPixel2;
    double deltaL, deltaA, deltaB;
    Vec2b deltaAB;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < width; k++)
            {
                for (int l = 0; l < height; l++)
                {
                    labPixel1 = cielabImg.at<Vec3b>(i, j);
                    labPixel2 = cielabImg.at<Vec3b>(k, l);

                    deltaL = labPixel1[LUMINANCE] - labPixel2[LUMINANCE];
                    deltaA = labPixel1[CHANNEL_A] - labPixel2[CHANNEL_A];
                    deltaB = labPixel1[CHANNEL_B] - labPixel2[CHANNEL_B];

                    deltaAB = Vec2b(static_cast<uchar>(deltaA), static_cast<uchar>(deltaB));
                    delta[static_cast<ulong>(i)][static_cast<ulong>(j)][static_cast<ulong>(k)][static_cast<ulong>(l)] = findDelta(deltaL, deltaAB, norm(deltaAB), parameters.theta, parameters.alpha);
                }
            }
        }
    }

    // Passo 3: otimizar
    // dado um conjunto de diferenças deltaij, encontramos uma imagem g em tons de cinza que minimiza a função f(g), definida como o somatório: para todo par ordenado de pixels (i, j) num conjunto K,
    // ((g(i) - g(j)) - deltaij)^2
    // inicializar g como o canal de luminância da imagem original
    // aqui não entendi como fazer nao

    return gray;
}
