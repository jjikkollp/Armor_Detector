#ifndef SOLVEPNP_H
#define SOLVEPNP_H

#include <opencv2/opencv.hpp>

const double MY_PI = 3.14159265358979323;

//pnp解算入口
cv::Vec3d solvepnp(cv::Mat &frame, std::vector<cv::Point2f> points);
//旋转矩阵转换为欧拉角
cv::Vec3d rm2euler(cv::Mat &rMat);
//旋转向量转换为欧拉角
cv::Vec3d rvec2euler(cv::Mat &rvec);
//x轴旋转
void rByX(double y,double z,double thetax,double &outy,double &outz);
//y轴旋转
void rByY(double x,double z,double thetay,double &outx,double &outz);
//z轴旋转
void rByZ(double x,double y,double thetaz,double &outx,double &outy);

#endif /* SOLVEPNP_H */