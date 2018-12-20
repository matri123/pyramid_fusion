#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;


void adb(Mat& img, bool b1, bool b2) {
    int rows = img.rows;
    int cols = img.cols;
    if (b1) {
        Mat temp1;
        img.row(rows-1).copyTo(temp1);
        img.push_back(temp1);
    }
    if (b2) {
        Mat temp2;
        img.col(cols-1).copyTo(temp2);
        hconcat(img, temp2, img);
    }
}

Mat selc(Mat img1, Mat img2, int ap){
    Mat ans;
    if (ap == 1){
    
    } else if (ap == 2){
    
    } else if (ap == 3){
        int um = 3;
        Mat kernel, A1, A2, sum;
        dilate(img1, A1, kernel);
        dilate(img2, A2, kernel);
        Mat comp = min(1, A1 > A2);

        Mat filter = Mat::ones(um, um, CV_32F);
        Point anchor(filter.cols-filter.cols/2-1, filter.rows-filter.rows/2-1);
        filter2D(comp, sum, comp.depth(), filter, anchor);
        Mat effi = min(1, sum > um*um/2);
        Mat effi1;
        effi.convertTo(effi1, img1.type());
        ans = img1.mul(effi1) + img2.mul(1-effi1);
    } else if (ap == 4){
    
    } else {
    
    }
    return ans;
}

Mat selb(Mat M1, Mat M2, int mp) {
    if (mp == 1) {
        return M1;
    } else if (mp == 2) {
        return M2;
    } else if (mp == 3) {
        return (M1 + M2) / 2;
    } else {
        return M1;
    }
}

Mat fusion(Mat M1, Mat M2, int zt, int ap, int mp) {
    int rows1 = M1.rows;
    int cols1 = M1.cols;
    int rows2 = M2.rows;
    int cols2 = M2.cols;

    // TODO exception error
    if (rows1 != rows2 || cols1 != cols2) {
        cout << "Not the same size!" << endl;
    }
    
    float eps = 1e-6;

    vector<Mat> E;
    vector<int> rows;
    vector<int> cols;

    for (int i1 = 1; i1 <= zt; i1++) {
        rows.push_back(M1.rows);
        cols.push_back(M1.cols);

		bool b1 = M1.rows%2 == 1;
		bool b2 = M1.cols%2 == 1;
		adb(M1, b1, b2);
        adb(M2, b1, b2);

        Mat G11, G21;
        Mat M1T, M2T;

        pyrDown(M1, G11);
        pyrUp(G11, M1T);
        pyrDown(M2, G21);
        pyrUp(G21, M2T);

        E.push_back(selc(M1/(M1T+eps), M2/(M2T+eps), ap));

        pyrDown(M1, M1);
        pyrDown(M2, M2);
    }

    M1 = selb(M1, M2, mp);

    for (int i = zt; i >= 1; i--) {
        Mat M1T;
        pyrUp(M1, M1T);
        M1 = (M1T + eps).mul(E[i - 1]);
        if (M1.rows != rows[i-1] || M1.cols != cols[i-1]) {
            M1 = M1(Range(0, rows[i-1]), Range(0, cols[i-1]));
        }
    }
    return M1;
}


