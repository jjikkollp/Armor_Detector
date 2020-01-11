#ifndef ARMOR_FINDER_H
#define ARMOR_FINDER_H

#include<opencv2/opencv.hpp>
#include "types.h"
#include "options.h"
#include "lb_finder.h"

//装甲板detector类
class Armor_Detector{
public:
    //装甲板识别主函数
    void work(cv::Mat &frame);
};

#endif /* ARMOR_FINDER_H */