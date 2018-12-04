#include "color2gray.h"

Mat rgb2cielab(Mat original)
{
    int width{original.cols}, height{original.rows};
    Mat cielab(height, width, CV_8UC3);

    cvtColor(original, cielab, COLOR_BGR2Lab);

    return cielab;
}

double crunch(double value, double alpha)
{
    if (alpha > 0)
        return alpha * tanh(value / alpha);
    else
        return 0;
}

double findDelta(double deltaL, double deltaA, double deltaB, double theta, double alpha)
{
    double delta;
    double deltaABNorm, dotproductABTheta;

    deltaABNorm = sqrt(pow(deltaA, 2) + pow(deltaB, 2));
    dotproductABTheta = deltaA * cos(theta) + deltaB * sin(theta);

    if (fabs(deltaL) > crunch(deltaABNorm, alpha))
        delta = deltaL;
    else if (dotproductABTheta >=0)
        delta = crunch(deltaABNorm, alpha);
    else
        delta = crunch(-deltaABNorm, alpha);

    return delta;
}

Mat color2gray(Mat original, ParameterHolder parameters)
{
    int width{original.cols}, height{original.rows}, N{width*height};

    // Passo 1: converter a imagem para o espaço CIE L*a*b*
    Mat cielabImg(height, width, CV_8UC3);
    cielabImg = rgb2cielab(original);

    // Passo 2: encontrar as target differences
    Vec3b labPixel1, labPixel2;
    double deltaL, deltaA, deltaB;

    vector<double> delta(static_cast<ulong>(N), 0);
    vector<double> luminance1d(static_cast<ulong>(N));
    vector<double> channelA1d(static_cast<ulong>(N));
    vector<double> channelB1d(static_cast<ulong>(N));

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            luminance1d[static_cast<ulong>(i * width + j)] = cielabImg.at<Vec3b>(i, j)[LUMINANCE];
            channelA1d[static_cast<ulong>(i * width + j)] = cielabImg.at<Vec3b>(i, j)[CHANNEL_A];
            channelB1d[static_cast<ulong>(i * width + j)] = cielabImg.at<Vec3b>(i, j)[CHANNEL_B];
        }
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            deltaL = luminance1d[static_cast<ulong>(i)] - luminance1d[static_cast<ulong>(j)];
            deltaA = channelA1d[static_cast<ulong>(i)] - channelA1d[static_cast<ulong>(j)];
            deltaB = channelB1d[static_cast<ulong>(i)] - channelB1d[static_cast<ulong>(j)];

            double d = findDelta(deltaL, deltaA, deltaB, parameters.theta, parameters.alpha);
            delta[static_cast<ulong>(i)] += d;
        }
    }

    // Passo 3: resolver a otimizacao
    Mat gray(height, width, CV_8UC3, Scalar(0));

    for (int i = 1; i < N; i++)
    {
        luminance1d[static_cast<ulong>(i)] = delta[static_cast<ulong>(i)] - delta[static_cast<ulong>(i-1)] + N * luminance1d[static_cast<ulong>(i-1)];
        luminance1d[static_cast<ulong>(i)] /= static_cast<double>(N);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Vec3b newValue;
            newValue[0] = newValue[1] = newValue[2] = static_cast<uchar>(luminance1d[static_cast<ulong>(i * width + j)]);
            gray.at<Vec3b>(i, j) = newValue;
        }
    }

    imshow("Gray", gray);
    waitKey(0);

    return gray;
}
