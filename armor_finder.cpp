#include "armor_finder.h"

void Armor_Detector::work(cv::Mat &frame){
    std::vector<LightBlob> LiBs; //存储图中所有可能的灯条
    std::vector<Armor_box> ArBs; //存储途中所有可能的装甲板

    //在图上寻找灯条
    
}