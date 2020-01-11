#include "types.h"
#include "armor_finder.h"
#include "options.h"

#define DBGLB //输出找灯条过程中的图像

bool Armor_Detector::findLightBlobs(cv::Mat &frame,std::vector<LightBlob> &LiBs){
    cv::Mat color_channel; //ENEMY颜色通道
    cv::Mat Vchannel; //灯条明度通道
    cv::Mat HSVframe; //转化为HSV空间的frame
    cv::Mat Binframe; //根据颜色二值化的frame
    std::vector<cv::Mat> channels; //存储不同的通道

    //使用颜色参数提取通道
    cv::split(frame,channels);
    if(ENEMY_COLOR==BLUE){
        color_channel = channels[0]; //B通道
    }else if(ENEMY_COLOR==RED){
        color_channel = channels[2]; //R通道
    }

#ifdef DBGLB
    cv::imshow("color_channel",color_channel);
    cv::waitKey(0);
#endif

    cv::cvtColor(frame,HSVframe,CV_BGR2HSV);
    cv::split(HSVframe,channels);
    Vchannel = channels[2]; //保存明度空间，和颜色一起确定灯条

    int light_threshold; //二值化图像的阈值（神秘的参数）

    if(ENEMY_COLOR==BLUE){
        light_threshold = 225; //神秘调参
    }else if(ENEMY_COLOR==RED){
        light_threshold = 200; //神秘调参
    }

    cv::threshold(color_channel,Binframe,light_threshold,255,CV_THRESH_BINARY);

#ifdef DBGLB
    cv::imshow("binary",Binframe);
    cv::waitKey(0);
#endif



    return true;
}