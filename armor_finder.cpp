#include "armor_finder.h"

//在返回前判断是否是OUTPUT模式
void ReturnFunc(cv::Mat frame){
    if(OUTPUT_MODE){
        cv::imshow("CurrentFrame",frame);
        cv::waitKey(0);
    }
}

void Armor_Detector::work(cv::Mat &frame){
    std::vector<LightBlob> LiBs; //存储图中所有可能的灯条
    std::vector<Armor_box> ArBs; //存储途中所有可能的装甲板

    //在图上寻找灯条
    if(!findLightBlobs(frame,LiBs)){
        ReturnFunc(frame);return;
    }
    
    return;
}