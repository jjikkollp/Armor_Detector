#include "types.h"
#include "armor_finder.h"
#include "options.h"

//#define DBGLB1 //输出找灯条过程中的图像
//#define DBGLB2 //输出找灯条过程中的图像
//#define DBGLB3 //输出处理后的三幅图像

//计算灯条外接矩形的长宽比
double length_width_ratio(cv::RotatedRect &rect){
    return rect.size.height>rect.size.width ? 
           rect.size.height/rect.size.width :
           rect.size.width/rect.size.height ;
}

//计算灯条和矩形的近似程度
double arearatio(std::vector<cv::Point> contour,const cv::RotatedRect rect){
    return cv::contourArea(contour)/rect.size.area();
}

//用两个参数的值排除噪点的干扰
bool isLightBlob(std::vector<cv::Point> contour,cv::RotatedRect rect){
    double lwr=length_width_ratio(rect);
    double ratio=arearatio(contour,rect);
    //小灯条可能外接矩形误差更大
    //std::cerr<<lwr<<" "<<ratio<<std::endl;
    return (lwr>1.2&&lwr<12)&&((rect.size.area()<50&&ratio>0.4)||(rect.size.area()>=50&&ratio>0.6));
}

//通过距离判断两个灯条是否等价，求交集用
bool isEqualLB(LightBlob lb1,LightBlob lb2){
    auto dist=lb1.LBRect.center-lb2.LBRect.center;
    return(dist.x*dist.x+dist.y*dist.y)<9;
}

void Armor_Detector::frameProcess(cv::Mat &frame){
    static cv::Mat kernel_erode = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(frame, frame, kernel_erode);

    static cv::Mat kernel_dilate = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(frame, frame, kernel_dilate);

    static cv::Mat kernel_dilate2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    dilate(frame, frame, kernel_dilate2);

    static cv::Mat kernel_erode2 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
    erode(frame, frame, kernel_erode2);
}

bool Armor_Detector::findLightBlobs(cv::Mat &frame,std::vector<LightBlob> &LiBs){
    cv::Mat color_channel; //ENEMY颜色通道
    cv::Mat Vchannel; //灯条明度通道
    cv::Mat HSVframe; //转化为HSV空间的frame
    cv::Mat Binframe,Binframe2; //根据颜色二值化的frame
    cv::Mat VBinframe; //根据V空间二值化的frame
    std::vector<cv::Mat> channels; //存储不同的通道

    //使用颜色参数提取通道（因为后面要修改channel，这里必须要深拷贝）
    cv::split(frame,channels);
    if(ENEMY_COLOR==BLUE){
        color_channel = channels[0].clone(); //B通道
    }else if(ENEMY_COLOR==RED){
        color_channel = channels[2].clone(); //R通道
    }

#ifdef DBGLB1
    cv::imshow("color_channel",color_channel);
    cv::waitKey(0);
#endif

    cv::cvtColor(frame,HSVframe,CV_BGR2HSV);
    cv::split(HSVframe,channels);
    Vchannel = channels[2].clone(); //保存明度空间，和颜色一起确定灯条


    //二值化图像

    int light_threshold; //二值化图像的阈值（神秘的参数）

    if(ENEMY_COLOR==BLUE){
        light_threshold = 225; //神秘调参
    }else if(ENEMY_COLOR==RED){
        light_threshold = 200; //神秘调参
    }

    //二值化颜色通道
    cv::threshold(color_channel,Binframe,light_threshold,255,CV_THRESH_BINARY);
    if(Binframe.empty()) return false; //没有找到任何一个灯条
    frameProcess(Binframe); //开闭运算，清除发光噪点

    //第二次二值化颜色通道
    cv::threshold(color_channel,Binframe2,175,255,CV_THRESH_BINARY);
    if(Binframe2.empty()) return false; //没有找到任何一个灯条
    frameProcess(Binframe2); //开闭运算，清除发光噪点

    //二值化V空间通道
    cv::threshold(Vchannel,VBinframe,200,255,CV_THRESH_BINARY);
    if(VBinframe.empty()) return false; //没有找到任何一个灯条
    frameProcess(VBinframe); //开闭运算，清除发光噪点

#ifdef DBGLB2
    cv::imshow("binary",VBinframe);
    cv::waitKey(0);
#endif

#ifdef DBGLB3
    cv::imshow("b1",Binframe);
    cv::waitKey(0);
    cv::imshow("b2",Binframe2);
    cv::waitKey(0);
    cv::imshow("bv",VBinframe);
    cv::waitKey(0);
#endif

    //三次操作，前两次减少二值化操作的误差，第三次通过明度降低蓝色背景带来的误差
    //对三次操作的图像取交得到正确的灯条集合

    std::vector<std::vector<cv::Point> > contour1,contour2,contourV; //灯条边缘集合
    std::vector<LightBlob> lbs1,lbs2,lbsv; //灯条集合
    std::vector<cv::Vec4i> hierarchy1,hierarchy2,hierarchyV; //hierarchy关系集合

    /*findContours*/
    /*
    //倒序排列降低vector删除复杂度
    sort(rm1.begin(),rm1.end(),[](int a,int b){return a>b;});
    sort(rm2.begin(),rm2.end(),[](int a,int b){return a>b;});
    sort(rmV.begin(),rmV.end(),[](int a,int b){return a>b;});

    for(auto x:rm1) lbs1.erase(lbs1.begin()+x);
    for(auto x:rm2) lbs2.erase(lbs2.begin()+x);
    for(auto x:rmV) lbsv.erase(lbsv.begin()+x);

    //加入到返回的灯条数组中
    for(auto x:lbs1) LiBs.emplace_back(x);
    for(auto x:lbs2) LiBs.emplace_back(x);
    for(auto x:lbsv) LiBs.emplace_back(x);
    */
    cv::findContours(Binframe,contour1,hierarchy1,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
    cv::findContours(Binframe2,contour2,hierarchy2,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
    cv::findContours(VBinframe,contourV,hierarchyV,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
    /*END*/

    //对三个图像分别做找灯条操作
    //fprintf(stderr,"contour1.size()=%u\n",contour1.size());

    for(int i=0;i<contour1.size();++i){
        if(hierarchy1[i][2]==-1){
            cv::RotatedRect rect=cv::minAreaRect(contour1[i]);
            if(isLightBlob(contour1[i],rect)){
                lbs1.emplace_back(LightBlob(rect,arearatio(contour1[i],rect)));
            }
        }
    }

    for(int i=0;i<contour2.size();++i){
        if(hierarchy2[i][2]==-1){
            cv::RotatedRect rect=cv::minAreaRect(contour2[i]);
            if(isLightBlob(contour2[i],rect)){
                lbs2.emplace_back(LightBlob(rect,arearatio(contour2[i],rect)));
            }
        }
    }

    for(int i=0;i<contourV.size();++i){
        if(hierarchyV[i][2]==-1){
            cv::RotatedRect rect=cv::minAreaRect(contourV[i]);
            if(isLightBlob(contourV[i],rect)){
                lbsv.emplace_back(LightBlob(rect,arearatio(contourV[i],rect)));
            }
        }
    }
    /* END */

    //对三个图像得到的灯条求交集
    std::vector<int> rm1,rm2,rmV;
    for(int i=0;i<lbs1.size();++i){
        for(int j=0;j<lbs2.size();++j){
            //fprintf(stderr,"%d %d\n",i,j);
            if(isEqualLB(lbs1[i],lbs2[j])){
                for(int k=0;k<lbsv.size();++k){
                    if(isEqualLB(lbs2[j],lbsv[k])){
                        //留下面积比值最大的
                        if(lbs1[i].area_ratio>lbs2[j].area_ratio){
                            if(lbs1[i].area_ratio>lbsv[k].area_ratio){
                                //rm2.emplace_back(j);
                                //rmV.emplace_back(k);
                                LiBs.emplace_back(lbs1[i]);
                            }else{
                                //rm1.emplace_back(i);
                                //rm2.emplace_back(j);
                                LiBs.emplace_back(lbsv[k]);
                            }
                        }else{
                            if(lbs2[j].area_ratio>lbsv[k].area_ratio){
                                //rm1.emplace_back(i);
                                //rmV.emplace_back(k);
                                LiBs.emplace_back(lbs2[j]);
                            }else{
                                //rm1.emplace_back(i);
                                //rm2.emplace_back(j);
                                LiBs.emplace_back(lbsv[k]);
                            }
                        }
                    }
                }
            }
        }
    }
    return LiBs.size()>=2;
}