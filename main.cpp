#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "fusion.cpp"

using namespace std;
using namespace cv;

int main()
{
    Mat img11 = imread("lake_IR.png", 0);
    Mat img22 = imread("lake_VIS.png", 0);
    Mat_<float> img1, img2;
    img11.convertTo(img1, CV_32F);
    img22.convertTo(img2, CV_32F);
    img1 /= 255.0;
    img2 /= 255.0;
    imshow("IR", img1);
    imshow("VIS", img2);

    Mat ans = fusion(img1, img2, 4, 3, 3);
    imshow("ans", ans);

    Mat output;
    ans *= 255;
    ans.convertTo(output, 0);
    imwrite("output.png", output);

    waitKey(0);

    return 0;
}
