#include "main.h"

cv::Mat frame; //要处理的每一帧图像。



bool Load_Frame(int type){
    if(type == 0){

    }
    else if(type == 1){

    }else{
        return false;// TODO:从云台摄像头读取。
    }
}

int main(int argc,char *argv[]){
    initopts(argc,argv);
    while(Load_Frame(CAMERA_TYPE)){

    }
    return 0;
}