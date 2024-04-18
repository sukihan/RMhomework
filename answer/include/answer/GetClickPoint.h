#pragma once
#include <cmath>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "rclcpp/rclcpp.hpp"

cv::Point2f GetClickPoint(cv::Mat img)
{
    auto logger = rclcpp::get_logger("GetClickPoint");
    cv::Point2f ClickPoint = cv::Point2f(-1,-1);
    if(img.empty())
    {   
        RCLCPP_ERROR(logger,"Image is empty!"); 
        return ClickPoint;
    }
    cv::Mat canny;

    Canny(img,canny,50,180);
    //核
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
    erode(canny, canny, element);

    std::vector<cv::Vec2f> lines;
    // 调整霍夫变换的参数
    HoughLines(canny,lines,1,CV_PI/180,120);
    
    if(lines.size() == 0)
    {
        RCLCPP_ERROR(logger,"No lines detected!");   
        return ClickPoint;
    }

    float r = lines[0][0], theta = lines[0][1];
    double a = cos(theta), b = sin(theta);

    std::vector<std::vector<cv::Point>> contours;
    findContours(canny, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++)
    {   
        cv::RotatedRect minRect = minAreaRect(contours[i]);

        if(minRect.size.area() > 1000 && minRect.size.area() < 2050 && minRect.size.width > 5 && minRect.size.height > 5)
        {
            cv::Point2f center = minRect.center;
            // 使用更精确的距离计算公式
            double delta = abs(a * center.x + b * center.y - r) / sqrt(a * a + b * b);
            if(delta < 15)
            {
                ClickPoint.x = minRect.center.x;
                ClickPoint.y = minRect.center.y;
            }
        }
    }
    
    return ClickPoint;
}