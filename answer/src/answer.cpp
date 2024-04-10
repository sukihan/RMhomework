#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/point32.hpp"
#include "opencv4/opencv2/opencv.hpp"
#include "cv_bridge/cv_bridge.h"
#include "answer/GetClickPoint.h"

class ImageSub:public rclcpp::Node
{
public:
    ImageSub():Node("answer")
    {
        sub = this->create_subscription<sensor_msgs::msg::Image>(
            "/raw_image", 10 , std::bind(&ImageSub::callback,this,std::placeholders::_1)
        );
        pub = this->create_publisher<geometry_msgs::msg::Point32>("/click_position",10);
    }
private:
    void callback(const sensor_msgs::msg::Image::SharedPtr image){
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(image,sensor_msgs::image_encodings::BGR8);
        cv::Mat& img = cv_ptr->image;
        cv::Point2f point = GetClickPoint(img);
        
        auto ClickPoint = std::make_shared<geometry_msgs::msg::Point32>();
        ClickPoint->x = point.x;
        ClickPoint->y = point.y;
        pub->publish(*ClickPoint);
    }
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub;
    rclcpp::Publisher<geometry_msgs::msg::Point32>::SharedPtr pub;
}; 

int main (int argc , char** argv)
{
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<ImageSub>());
    rclcpp::shutdown();
    return 0;
}