#ifndef ATTACK_H
#define ATTACK_H

#include<opencv2/opencv.hpp>
#include "types.h"
#include "solvepnp.h"

class Attacker{
public:
    double yaw; //yaw 向右为正
    double pitch; //pitch 向上为正
    cv::Vec3d vec; //相对方向向量
    Armor_box aim;
    //构造函数
    Attacker()=default;
    //计算角度
    void solveangle(cv::Mat &frame);
    //attacker入口
    void run(cv::Mat &frame,Armor_box aim);
};

#endif /* ATTACK_H */