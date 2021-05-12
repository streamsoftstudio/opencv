//
//  main.cpp
//  OpenCVPlayground
//
//  Created by Andrija Milovanovic on 2/8/21.
//

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int drawCircle(std::vector<cv::Vec3f>& circle, Mat& image, Scalar color);

int main(int argc, const char * argv[])
{
    if( argc != 2)
    {
         cout <<"Provide path to image" << endl;
         return -1;
    }
    
    Mat src;
    
    src = imread(argv[1], IMREAD_COLOR);

    if(! src.data )
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );
    
    // show initial image
    cout <<  "Original image" << std::endl ;
    imshow( "Display window", src );
    waitKey(0);

    // 1. create image with circle "window"
    cv::Mat image(src.size(),src.type(),Scalar(255,255,255));
    cv::Mat mask(src.size(),CV_8UC1);
    circle(mask, cv::Point(mask.cols/2, mask.rows/2+40), mask.cols/4, cv::Scalar(255, 0, 0), FILLED, 8, 0);
    src.copyTo(image,mask);
    


    // 2. detect dark areas
    Mat mask2(src.size(),CV_8UC1);
    cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(image, image, cv::Size(11, 11), 0);
    cv::threshold( image, mask2, 60 , 255 , THRESH_BINARY_INV );
    
    
    cout <<  "Original image" << std::endl ;
    imshow( "Display window", image );
    waitKey(0);
    
    cout <<  "Original image" << std::endl ;
    imshow( "Display window", mask2 );
    waitKey(0);
    
    vector<vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
    cv::findContours(mask2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    cv::fillPoly(mask2, contours, Scalar(255,255,255));
    
    cout <<  "Original image" << std::endl ;
    imshow( "Display window", mask2 );
    waitKey(0);
    
    src.copyTo(image,mask2);
    Mat blured;
    medianBlur(image, blured, 3);

    
    cv::Mat hsv_image;
    cv::cvtColor(blured, hsv_image, cv::COLOR_BGR2HSV);
    
    std::vector<cv::Vec3f> redCircles, blueCircles, greenCircles, yellowCircle, cyanCircle, whiteCircle, violeteCircle;
    cv::Mat lower_red_hue_range, upper_red_hue_range, red_hue_range;
    cv::Mat blue_hue_range, green_hue_range, yellow_hue_range, cyan_range, white_range, violete_range;
    
    int rc(0), gc(0), bc(0), yc(0), cc(0), wc(0), vc(0);
    
    // red
    cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), upper_red_hue_range);
    cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_range);
    cv::GaussianBlur(red_hue_range, red_hue_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(red_hue_range, redCircles, HOUGH_GRADIENT, 1, red_hue_range.rows/8, 200, 20, 10, 60);
    rc = drawCircle(redCircles, src, cv::Scalar(255, 255, 0));// [self drawCircle:redCircles image:src color: cv::Scalar(0, 255, 255)];
    
    // green
    cv::inRange(hsv_image, cv::Scalar(40, 40, 40), cv::Scalar(80, 255, 255), green_hue_range); //70
    cv::GaussianBlur(green_hue_range, green_hue_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(green_hue_range, greenCircles, HOUGH_GRADIENT, 1, green_hue_range.rows/8, 200, 20, 10, 60);
    gc = drawCircle(greenCircles, src, cv::Scalar(255, 255, 0));// [self drawCircle:greenCircles image:src color: cv::Scalar(255, 255, 0)];

    
    // blue
    cv::inRange(hsv_image, cv::Scalar(110, 50, 50), cv::Scalar(135, 255, 255), blue_hue_range); //120
    cv::GaussianBlur(blue_hue_range, blue_hue_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(blue_hue_range, blueCircles, HOUGH_GRADIENT, 1, blue_hue_range.rows/8, 200, 20, 10, 60);
    bc = drawCircle(blueCircles, src, cv::Scalar(255, 255, 0)); //[self drawCircle:blueCircles image:src color: cv::Scalar(255, 0, 255)];
    
    // yellow
    cv::inRange(hsv_image, cv::Scalar(10, 50, 50), cv::Scalar(30, 255, 255), yellow_hue_range); // 20-30
    cv::GaussianBlur(yellow_hue_range, yellow_hue_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(yellow_hue_range, yellowCircle, HOUGH_GRADIENT, 1, yellow_hue_range.rows/8, 200, 20, 10, 60);
    yc = drawCircle(yellowCircle, src, cv::Scalar(255, 255, 0)); //[self drawCircle:yellowCircle image:src color: cv::Scalar(255, 255, 0)];
    
    // cyan
    cv::inRange(hsv_image, cv::Scalar(85, 50, 50), cv::Scalar(95, 255, 255), cyan_range); // 20-30
    cv::GaussianBlur(cyan_range, cyan_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(cyan_range, cyanCircle, HOUGH_GRADIENT, 1, cyan_range.rows/8, 200, 20, 10, 60);
    cc = drawCircle(cyanCircle, src, cv::Scalar(255, 255, 0)); //[self drawCircle:cyanCircle image:src color: cv::Scalar(255, 255, 0)];
    
    // volete
    cv::inRange(hsv_image, cv::Scalar(110, 30, 230), cv::Scalar(170, 45, 255), violete_range);
    cv::GaussianBlur(violete_range, violete_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(violete_range, violeteCircle, HOUGH_GRADIENT, 1, violete_range.rows/8, 200, 20, 10, 60);
    vc = drawCircle(violeteCircle, src, cv::Scalar(255, 255, 0)); // s[self drawCircle:violeteCircle image:src color: cv::Scalar(255, 255, 0)];
    
    // white
    cv::inRange(hsv_image, cv::Scalar(0, 0, 230), cv::Scalar(255, 25, 255), white_range);
    cv::GaussianBlur(white_range, white_range, cv::Size(9, 9), 2, 2);
    cv::HoughCircles(white_range, whiteCircle, HOUGH_GRADIENT, 1, white_range.rows/8, 200, 20, 10, 60);
    
    
    wc = drawCircle(whiteCircle, src, cv::Scalar(255, 255, 0)); //[self drawCircle:whiteCircle image:src color: cv::Scalar(255, 255, 0)];
    
    cout <<  "Original image" << std::endl ;
    imshow( "Display window", white_range );
    waitKey(0);
    
}

int drawCircle(std::vector<cv::Vec3f>& circle, Mat& image, Scalar color)
{
    int counter = 0;
    for(size_t current_circle = 0; current_circle < circle.size(); ++current_circle)
    {
        cv::Point center(std::round(circle[current_circle][0]), std::round(circle[current_circle][1]));
        int radius = std::round(circle[current_circle][2]);
        if( radius > 0) {
            cv::circle(image, center, radius,color, 3);
            counter = counter + 1;
        }
    }
    return counter;
}

