#include "types.h"

cv::Point2f Armor_box::Center(){
    return cv::Point2f(ABRect.x+ABRect.width/2,ABRect.y+ABRect.height/2);
}