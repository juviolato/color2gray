#ifndef COLOR2GRAY_H
#define COLOR2GRAY_H

#include <iostream>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

const double PI = 3.14159265359;
const int WHOLE_NEIGHBORHOOD = 0;
const int LUMINANCE = 0;
const int CHANNEL_A = 1;
const int CHANNEL_B = 2;

struct ParameterHolder
{
    double theta;
    double alpha;
    double mu;

    ParameterHolder()
    {
        theta = PI/4.0;
        alpha = 10.0;
        mu = WHOLE_NEIGHBORHOOD;
    }

    ParameterHolder(double t, double a, double m)
    {
        theta = t;
        alpha = a;
        mu = m;
    }
};

Mat rgb2cielab(Mat original);

double crunch(double value, double alpha);

double dotProduct2(Vec2b v, Vec2b u);

double findDelta(double deltaL, Vec2b deltaAB, double deltaABNorm, double theta, double alpha);

Mat color2gray(Mat original, ParameterHolder parameters);

#endif // COLOR2GRAY_H
