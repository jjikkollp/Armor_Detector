#ifndef TYPES_H
#define TYPES_H

#include<opencv2/opencv.hpp>

//灯条类定义
class LightBlob{
public:
    cv::RotatedRect LBRect; //代表灯条的矩形。
    double area_ratio; //灯条和矩形的近似程度
    //构造函数
    LightBlob(cv::RotatedRect r,double ratio):LBRect(r),area_ratio(ratio){}
    //LightBlob()=default;
};

//装甲板类定义
class Armor_box{
public:
    cv::Rect2d ABRect; //装甲板矩形
    std::vector<LightBlob> LBS; //装甲板对应的灯条们
    int id; //装甲板的数字
    //构造函数
    Armor_box(cv::Rect2d Rect,std::vector<LightBlob> LB):ABRect(Rect),LBS(LB){}
    cv::Point2f Center(); //获取装甲板中心
};

#endif /* TYPES_H */