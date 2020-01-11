#include "main.h"

/********* 宏定义区************/

//#define HSVTEST //测试HSV空间下的表现效果

/************结束*************/

cv::Mat frame; //要处理的每一帧图像。

std::string FILEPATH="../pos/"; //存储装甲板图像的文件相对路径
cv::VideoCapture cam(0); //laptop摄像头初始化
int Current_Frame=1; //读取文件时的计数器。
Armor_Detector arm_det; //寻找装甲ban实例对象

//读取每一帧，返回是否成功读取
bool Load_Frame(int type){
    if(type == 0){
        if(Current_Frame > 667) return false; // 文件读取完了
        std::string file = FILEPATH+"a"+std::to_string(Current_Frame)+".jpg";
        frame = cv::imread(file);
        ++Current_Frame;
        return true;
    }
    else if(type == 1){
        cam >> frame;
        return true;
    }else{
        return false;// TODO:从云台摄像头读取。
    }
}

int main(int argc,char *argv[]){
    initopts(argc,argv);
    while(Load_Frame(CAMERA_TYPE)){
        arm_det.work(frame);
#ifdef HSVTEST
        cv::cvtColor(frame,frame,CV_BGR2HSV);
        std::vector<cv::Mat> channels;
        cv::split(frame,channels);
        cv::imshow("testboxH",channels[0]);
        cv::waitKey(0);
        cv::imshow("testboxS",channels[1]);
        cv::waitKey(0);
        cv::imshow("testboxV",channels[2]);
        cv::waitKey(0);
#endif
    }
    fprintf(stderr,"The Program ended successfully!");
    return 0;
}