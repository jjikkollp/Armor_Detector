#ifndef OPTIONS_H
#define OPTIONS_H

enum color{
    BLUE,RED
};
extern int ENEMY_COLOR;
extern int CAMERA_TYPE;
extern int OUTPUT_MODE;

void initopts(int argc,char **argv);


#endif /* OPTIONS_H */