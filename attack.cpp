#include "attack.h"

void Attacker::solveangle(cv::Mat &frame){
    vec = solvepnp(frame,aim.points);
    double x = vec[0];
    double y = vec[1];
    double z = vec[2];
    yaw = atan2(x,z);
    pitch = atan2(y,z);
}

void Attacker::run(cv::Mat &frame,Armor_box aim_){
    aim = aim_;
    solveangle(frame);
       
}