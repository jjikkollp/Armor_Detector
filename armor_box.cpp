#include "types.h"

cv::Point2f Armor_box::Center(){
    return cv::Point2f(ABRect.x+ABRect.width/2,ABRect.y+ABRect.height/2);
}

std::vector<cv::Point2f> Armor_box::points(){
    std::vector<cv::Point2f> pois;
    pois.emplace_back(ABRect.x,ABRect.y);
    pois.emplace_back(ABRect.x,ABRect.y+ABRect.height);
    pois.emplace_back(ABRect.x+ABRect.width,ABRect.y+ABRect.height);
    pois.emplace_back(ABRect.x+ABRect.width,ABRect.y);
    return pois;
}