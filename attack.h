#ifndef ATTACK_H
#define ATTACK_H

#include<opencv2/opencv.hpp>

class Attacker{
    double yaw;
    double pitch;
    cv::Vec3d vec; //相对方向向量
    void run();
};

#endif /* ATTACK_H */