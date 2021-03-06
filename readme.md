#### 一.代码运行环境

Ubuntu18.04（运行在外置SSD上）

Intel(R) Core(TM) i5-5257U CPU @ 2.70GHz 双核四线程

8GB 1867MHz DDR3

运行效率：
667张文件，平均每帧40ms //lbfinder.cpp(包括HSV空间，三种求交集)
667张文件，平均每帧20ms //lbfinder2.cpp(两种求交集)
(可以在CMakeLists.txt里面改文件)

#### 二.运行方式

数据集的pos文件放在项目目录下

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./arm_detector
```

可选命令行参数：见帮助手册(./arm_detector --help)

#### 三.运行流程

主函数内除了计时和读取文件，只调用了`Armor_Detector::work`。

在types.h中定义了Armor_box类和LightBlob类

Armor_Dectector类在Armor_finder.h中定义。

在armor_finder.cpp中实现主要操作，流程为：调用findLightBlobs找到图中所有的灯条，调用matchLBS对灯条进行匹配，框出装甲板。

#### 四.实现细节

lb_finder.cpp实现了找到灯条的所有过程。

首先将图像分出B通道（R通道），使用两个不同的阈值将图像二值化，之后对图像中的轮廓求交集，目的是减少环境光源的影响。

同时将图像转化到HSV空间，提取V通道，二值化，再求交集。目的是去除其他不发光的蓝色/红色物体的干扰。

之后对找到的灯条，回到原图中再次检验BGR颜色，避免有(0,255,255)这样黄色光的干扰。

armor_finder.cpp中实现了配对灯条找到装甲板的过程。

首先将找到的灯条按照坐标排序，顺序匹配，每个灯条从距离他最近的灯条开始尝试匹配。

匹配分为5个判断部分，1.两个灯条在图上的长度相差不大，2.两个灯条几乎平行，3.两个灯条的水平距离和灯条的比值不能过大，4.通过中点连线的角度和两个灯条的角度判断两个灯条是否虽然平行但错开太多，5.计算如果是这样一组灯条，装甲板的倾斜角度。

