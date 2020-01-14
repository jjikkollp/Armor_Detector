#ifndef ARMOR_FINDER_H
#define ARMOR_FINDER_H

#include<opencv2/opencv.hpp>
#include "types.h"
#include "options.h"

//装甲板detector类
class Armor_Detector{
public:
    //匹配灯条函数，找到装甲板
    bool matchLBS(cv::Mat &frame, std::vector<LightBlob> &LiBs, std::vector<Armor_box> &ArBs);
    //在图上寻找所有的灯条
    bool findLightBlobs(cv::Mat &frame, std::vector<LightBlob> &LiBs);
    //对图像进行开闭运算函数
    void frameProcess(cv::Mat &frame);
    //装甲板识别主函数
    std::vector<cv::Point2f> work(cv::Mat &frame);
};

#endif /* ARMOR_FINDER_H */