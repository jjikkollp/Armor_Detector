#ifndef ARMOR_FINDER_H
#define ARMOR_FINDER_H

#include<opencv2/opencv.hpp>
#include "types.h"
#include "options.h"

//装甲板detector类
class Armor_Detector{
public:
    //在图上寻找所有的灯条
    bool findLightBlobs(cv::Mat &frame, std::vector<LightBlob> &LiBs);
    //装甲板识别主函数
    void work(cv::Mat &frame);
};

#endif /* ARMOR_FINDER_H */