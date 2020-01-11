#include "options.h"
#include <map>

int ENEMY_COLOR=0; //0表示蓝色 1表示红色
int CAMERA_TYPE=0; //0表示从文件读取，1表示调用本机摄像头，TODO：调用云台上的摄像头

//定义一个map存储参数列表
std::map<std::string, std::pair<std::string, void(*)(void) > > opts={
    {"--help",{
            "show the help information",[](){
                for(auto &opt : opts){
                    fprintf(stderr,"%s   %s\n",opt.first,opt.second.first);
                }
            }
        }
    },
    {"--setcolorb",{
            "set enemy's color to blue, this argument can be omitted",[](){
                ENEMY_COLOR=0;
                fprintf(stderr,"successfully set enemy's color to blue!");
            }
        }
    },
    {"--setcolorr",{
            "set enemy's color to red while the default is blue",[](){
                ENEMY_COLOR=1;
                fprintf(stderr,"successfully set enemy's color to red!");
            }
        }
    },
    {"--setcamera0",{
            "get frame from files, this argument can be omitted",[](){
                CAMERA_TYPE=0;
                fprintf(stderr,"successfully set frame loading file!");
            }
        }
    },
    {"--setcamera1",{
            "get frame from laptop camera while the default is loadind from files",[](){
                CAMERA_TYPE=1;
                fprintf(stderr,"successfully set laptop camera!");
            }
        }
    }
};

//从命令行参数列表初始化
void initopts(int argc,char **argv){
    if(argc>=2){
        for(int i=1;i<argc;++i){
            auto ans=opts.find(std::string(argv[i]));
            if(ans==opts.end()){
                fprintf(stderr,"Unknown option. Plz use --help to get the manual");
            }else{
                ans->second.second();
            }
        }
    }
}