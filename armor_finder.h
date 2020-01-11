#ifndef ARMOR_FINDER_H
#define ARMOR_FINDER_H

#include<opencv2/opencv.hpp>
#include "types.h"

//装甲板detector类
class Armor_Detector{
public:
    void work(cv::Mat &frame);
};

#endif /* ARMOR_FINDER_H */