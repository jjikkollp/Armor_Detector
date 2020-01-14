#include "solvepnp.h"

#define draw //绘制定位点和坐标系

cv::Vec3d rm2euler(cv::Mat &R){
    double sy = sqrt(R.at<double>(0,0)*R.at<double>(0,0)+R.at<double>(1,0)*R.at<double>(1,0));
    bool singular = sy < 1e-6;
    double x,y,z;
    if (!singular){
        x = atan2(R.at<double>(2,1),R.at<double>(2,2));
        y = atan2(-R.at<double>(2,0),sy);
        z = atan2(R.at<double>(1,0),R.at<double>(0,0));
    }
    else{
        x = atan2(-R.at<double>(1,2),R.at<double>(1,1));
        y = atan2(-R.at<double>(2,0),sy);
        z = 0;
    }
    return cv::Vec3d(x,y,z);
}

cv::Vec3d rvec2euler(cv::Mat &rvec){
    cv::Mat rMat(3,3,CV_64F); //旋转矩阵
    cv::Rodrigues(rvec,rMat);
    cv::Vec3d euler = rm2euler(rMat);
    return euler;
}

void rByX(double y,double z,double thetax,double &outy,double &outz){
    double y1=y,z1=z;
    double rx=thetax*MY_PI/180;
    outy=cos(rx)*y1-sin(rx)*z1;
    outz=cos(rx)*z1+sin(rx)*y1;
}

void rByY(double x,double z,double thetay,double &outx,double &outz){
    double x1=x,z1=z;
    double ry=thetay*MY_PI/180;
    outx=cos(ry)*x1+sin(ry)*z1;
    outz=cos(ry)*z1-sin(ry)*x1;
}

void rByZ(double x,double y,double thetaz,double &outx,double &outy){
    double x1=x,y1=y;
    double rz=thetaz*MY_PI/180;
    outx=cos(rz)*x1-sin(rz)*y1;
    outy=sin(rz)*x1+cos(rz)*y1;
}


//SOLVE_PNP,传入图像中四个位置点
cv::Vec3d solvepnp(cv::Mat &frame,std::vector<cv::Point2d> points){
    if(points.size()!=4){
        fprintf(stderr,"Plz input 4 points\n");
        return;
    }
#ifdef draw
    for(int i=0;i<points.size();++i){
        cv::line(frame,points[i],points[(i+1)%4],cv::Scalar(255,255,255),2);
    }
#endif

    //图像坐标
    std::vector<cv::Point2d> Points2D;
    Points2D.emplace_back(points[0]);
    Points2D.emplace_back(points[1]);
    Points2D.emplace_back(points[2]);
    Points2D.emplace_back(points[3]);

    //世界坐标
    std::vector<cv::Point3d> Points3D;
    //wait to be set


    //wait to be set
    double fx,cx,fy,cy; //相机内参
    double c1,c2; //畸变参数

    cv::Mat cameraMatrix = (cv::Mat1d(3,3) << fx,0,cx,0,fy,cy,0,0,1);
    cv::Mat distCoeffs = (cv::Mat1d(1,4) << c1,c2,0,0);
    //相机=r世界+t
    cv::Mat rvec = cv::Mat::zeros(3,1,CV_64F); //旋转矩阵
    cv::Mat tvec = cv::Mat::zeros(3,1,CV_64F); //平移矩阵
    cv::solvePnP(Points3D,Points2D,cameraMatrix,distCoeffs,rvec,tvec);

    //绘制坐标系
#ifdef draw
    std::vector<cv::Point3d> center_points;
    center_points.emplace_back(cv::Point3d(0,0,0));
    center_points.emplace_back(cv::Point3d(100,0,0));
    center_points.emplace_back(cv::Point3d(0,100,0));
    center_points.emplace_back(cv::Point3d(0,0,100));
    std::vector<cv::Point2d> img_points;
    cv::projectPoints(center_points,rvec,tvec,cameraMatrix,distCoeffs,img_points);

    cv::line(frame,img_points[0],img_points[1],cv::Scalar(0,0,255),3);
    cv::line(frame,img_points[0],img_points[2],cv::Scalar(0,255,0),3);
    cv::line(frame,img_points[0],img_points[3],cv::Scalar(255,0,0),3);
#endif

    cv::Vec3d euler = rvec2euler(rvec);
    double theta_x = euler[0]*180/MY_PI;
    double theta_y = euler[1]*180/MY_PI;
    double theta_z = euler[2]*180/MY_PI;

    double x = tvec.ptr<double>(0)[0];
    double y = tvec.ptr<double>(0)[1];
    double z = tvec.ptr<double>(0)[2];

    rByZ(x,y,-theta_z,x,y);
    rByY(x,z,-theta_y,x,z);
    rByX(y,z,-theta_x,y,z);
    //x=-x;y=-y;z=-z;
    //fprintf(stderr,"摄像头相对于世界坐标系的坐标为x=%.3f y=%.3f z=%.3f\n",x,y,z);
    fprintf(stderr,"目标相对摄像头的向量为 (%.3f %.3f %.3f)\n",x,y,z);
    
}