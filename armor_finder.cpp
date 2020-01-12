//TODO：获取装甲板ID
#include "armor_finder.h"

//#define DBGLBS //输出灯条框
// #define LB_PARAM //调试灯条匹配参数

//在返回前判断是否是OUTPUT模式
void ReturnFunc(cv::Mat frame){
    if(OUTPUT_MODE){
        cv::imshow("CurrentFrame",frame);
        cv::waitKey(0);
    }
}

//DEL:倾斜装甲板无法判断，故删除
//判断两个灯条的水平高度差是否较小
// bool heightDiff(LightBlob &x,LightBlob &y){
    // return abs((x.LBRect.center-y.LBRect.center).y)<40;
// }


//判断两个灯条的长度是否接近（考虑一个较远，一个较近的情况，存在一个区间）
bool lengthDiff(LightBlob &x,LightBlob &y){
    double ratio = x.length/y.length;
    return ratio<2.5&&ratio>0.4;
}

//判断两个灯条的竖直角度差是否较小
bool angleDiff(LightBlob &x,LightBlob &y){
    double angx = (x.LBRect.size.width > x.LBRect.size.height) ? (x.LBRect.angle):(x.LBRect.angle-90);
    double angy = (y.LBRect.size.width > y.LBRect.size.height) ? (y.LBRect.angle):(y.LBRect.angle-90);
    return abs(angx-angy)<20;
}

//判断两个灯条的水平间距
bool distDiff(LightBlob &x,LightBlob &y){
    cv::Point2f center = x.LBRect.center-y.LBRect.center;
    double dist=sqrt(center.ddot(center));
    //判断灯条长度和距离的比值
    double ratio = dist/x.length;
    return (ratio<10&&ratio>0.5);
}

//判断两个灯条的错位度（SJTU的代码）
static bool CuoWeiDuJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    float angle_i = light_blob_i.LBRect.size.width > light_blob_i.LBRect.size.height ? light_blob_i.LBRect.angle :
                    light_blob_i.LBRect.angle - 90;
    float angle_j = light_blob_j.LBRect.size.width > light_blob_j.LBRect.size.height ? light_blob_j.LBRect.angle :
                    light_blob_j.LBRect.angle - 90;
    float angle = (angle_i + angle_j) / 2.0 / 180.0 * 3.14159265459;
    if (abs(angle_i - angle_j) > 90) {
        angle += 3.14159265459 / 2;
    }
    cv::Vec2f orientation(cos(angle), sin(angle));
    cv::Vec2f p2p(light_blob_j.LBRect.center.x - light_blob_i.LBRect.center.x,
                 light_blob_j.LBRect.center.y - light_blob_i.LBRect.center.y);
    return abs(orientation.dot(p2p)) < 25;
}
//判断装甲板的方向（SJTU的代码）
static bool boxAngleJudge(const LightBlob &light_blob_i, const LightBlob &light_blob_j) {
    float angle_i = light_blob_i.LBRect.size.width > light_blob_i.LBRect.size.height ? light_blob_i.LBRect.angle :
                    light_blob_i.LBRect.angle - 90;
    float angle_j = light_blob_j.LBRect.size.width > light_blob_j.LBRect.size.height ? light_blob_j.LBRect.angle :
                    light_blob_j.LBRect.angle - 90;
    float angle = (angle_i + angle_j) / 2.0;
    if (abs(angle_i - angle_j) > 90) {
        angle += 90.0;
    }
    return (-120.0 < angle && angle < -60.0) || (60.0 < angle && angle < 120.0);
}

//综合这些函数判断两个灯条是否match
bool isMatchLB(LightBlob &x,LightBlob &y){
#ifdef LB_PARAM
    fprintf(stderr,"lenghtratio = %.3f\n",x.length/y.length);
    fprintf(stderr,"%d %d %d\n",(int)lengthDiff(x,y),(int)angleDiff(x,y),(int)distDiff(x,y));
#endif
    return lengthDiff(x,y)&&angleDiff(x,y)&&distDiff(x,y)&&boxAngleJudge(x,y)&&CuoWeiDuJudge(x,y);
}

bool Armor_Detector::matchLBS(cv::Mat &frame, std::vector<LightBlob> &LiBs,std::vector<Armor_box> &ArBs){
    for(int i=0;i<LiBs.size()-1;++i){
        for(int j=i+1;j<LiBs.size();++j){
            if(!isMatchLB(LiBs[i],LiBs[j])) continue;
            cv::Rect2d l=LiBs[i].LBRect.boundingRect(); //得到左右灯条代表的矩形
            cv::Rect2d r=LiBs[j].LBRect.boundingRect(); //得到左右灯条代表的矩形

            //计算装甲板四个点的坐标
            double mxx,mxy,mnx,mny;
            mnx=std::min(l.x,r.x)-3;
            mxx=std::max(l.x+l.width,r.x+r.width)+3;
            mny=std::min(l.y,r.y)-(l.height+r.height)/4.;
            mxy=std::max(l.y+l.height,r.y+r.height)+(l.height+r.height)/4.;

            //防止越界
            if(mnx<0||mxx>frame.cols||mny<0||mxy>frame.rows) continue;

            //存储装甲板
            ArBs.emplace_back(
                Armor_box(
                    cv::Rect2d(mnx,mny,mxx-mnx,mxy-mny),
                    {LiBs[i],LiBs[j]}
                )
            );
            break;
        }
    }
    if(!ArBs.size()) return false;
    else return true;
}

void Armor_Detector::work(cv::Mat &frame){
    std::vector<LightBlob> LiBs; //存储图中所有可能的灯条
    std::vector<Armor_box> ArBs; //存储途中所有可能的装甲板

    LiBs.clear();
    ArBs.clear();

    //在图上寻找灯条
    if(!findLightBlobs(frame,LiBs)){
        ReturnFunc(frame);return;
    }

    //fprintf(stderr,"%d\n",(int)LiBs.size());

#ifdef DBGLBS
    for(auto x:LiBs){
        cv::Point2f* verts = new cv::Point2f[4];
        x.LBRect.points(verts);
        for(int i=0;i<4;++i){
            cv::line(frame,verts[i],verts[(i+1)%4],cv::Scalar(0,255,255),3);
        }
    }
    cv::imshow("test_bbox_for_LB",frame);
    cv::waitKey(0);
#endif

    //配对灯条，找出装甲板
    if(!matchLBS(frame,LiBs,ArBs)){
        ReturnFunc(frame);return;
    }

    if(OUTPUT_MODE){
        //draw armors
        for(auto x:ArBs){
            cv::rectangle(frame,x.ABRect,cv::Scalar(0,255,255),2);
        }
    }

    ReturnFunc(frame);
    return;
}