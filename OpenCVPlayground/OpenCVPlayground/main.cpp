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
    
    src = imread(argv[1], IMREAD_UNCHANGED);

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
    
    
    Mat mask(src.size(),CV_8UC1);
    circle(mask, cv::Point(mask.cols/2, mask.rows/2), 120, cv::Scalar(255, 0, 0), -1, 8, 0);
    Mat image(src.size(),CV_8UC3,Scalar(255,255,255));
    src.copyTo(image,mask);
    
    Mat blured;
    medianBlur(image, blured, 3);
    
    cout << "Blured and masked image" << std::endl ;
    imshow( "Display window", blured );
    waitKey(0);
     
    Mat hsv_image;
    cvtColor(blured, hsv_image, cv::COLOR_BGR2HSV);
    
    Mat lower_red_hue_range, blue_hue_range, green_hue_range;
    Mat upper_red_hue_range;
    inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
    inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), upper_red_hue_range);
    
    inRange(hsv_image, cv::Scalar(40, 40, 40), cv::Scalar(80, 255, 255), green_hue_range); //70
    inRange(hsv_image, cv::Scalar(110, 50, 50), cv::Scalar(135, 255, 255), blue_hue_range); //120
    
    Mat red_hue_image;
    addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
    GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);
    GaussianBlur(blue_hue_range, blue_hue_range, cv::Size(9, 9), 2, 2);
    GaussianBlur(green_hue_range, green_hue_range, cv::Size(9, 9), 2, 2);
    
    cout << "Red image" << std::endl ;
    imshow( "Display window", red_hue_image );
    waitKey(0);
    cout << "Green image" << std::endl ;
    imshow( "Display window", green_hue_range );
    waitKey(0);
    cout << "Blue image" << std::endl ;
    imshow( "Display window", blue_hue_range );
    waitKey(0);
    
    std::vector<cv::Vec3f> redCircles, blueCircles, greenCircles;
    HoughCircles(red_hue_image, redCircles, HOUGH_GRADIENT, 1, red_hue_image.rows/8, 100, 50, 0, 0);
    HoughCircles(blue_hue_range, blueCircles, HOUGH_GRADIENT, 1, blue_hue_range.rows/8, 100, 50, 0, 0);
    HoughCircles(green_hue_range, greenCircles, HOUGH_GRADIENT, 1, green_hue_range.rows/8, 100, 50, 0, 0);
    
    int r = drawCircle(redCircles, src, Scalar(0, 255, 255));
    int g = drawCircle(blueCircles, src, Scalar(255, 0, 255));
    int b = drawCircle(greenCircles, src, Scalar(255, 255, 0));
    
    cout << "Final image red: " << r << " green: " << g << " blue: " << b << std::endl ;
    imshow( "Display window", blue_hue_range );
    
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

