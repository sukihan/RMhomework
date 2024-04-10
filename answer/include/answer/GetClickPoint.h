#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Point2f GetClickPoint(Mat img){
    Point2f ClickPoint = Point2f(-1,-1);
    if(img.empty()){
        cout << "Image is empty" << endl;
        cout << "CLick x :" << ClickPoint.x << " y : " << ClickPoint.y << endl;
        return ClickPoint;
    }
    cout << img.size() << endl;
    Mat canny;

    Canny(img,canny,50,200);
    vector<Vec2f> lines;
    HoughLines(canny,lines,1,CV_PI/180,50);
    
    if(lines.size() == 0){
        cout << "No lines detected" << endl;
        cout << "CLick x :" << ClickPoint.x << " y : " << ClickPoint.y << endl;
        return ClickPoint;
    }

    float r = lines[0][0], theta = lines[0][1];
    double a = cos(theta), b = sin(theta);

    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++)
    {
        RotatedRect minRect = minAreaRect(contours[i]);

        Point2f rectPoints[4];
        minRect.points(rectPoints);
        if(minRect.size.area() > 1000 && minRect.size.area() < 2050){
            Point2f center = minRect.center;
            int delta = abs((center.x * a + center.y * b - r) / (a*a + b*b));
            if(delta < 14){
                ClickPoint.x = center.x;
                ClickPoint.y = center.y;
                cout << "CLick x :" << ClickPoint.x << " y : " << ClickPoint.y << endl;
            }
        }
    }
    
    return ClickPoint;
}