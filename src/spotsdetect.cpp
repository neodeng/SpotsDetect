/*! @file
********************************************************************************
<PRE>
模块名: 光点探测
文件名: spotsdetect.h
相关文件: spotsdetect.cpp
文件实现功能: 1.静态图片的光点探测，准确获得光点数量，中心亮度，坐标和近似半径，并输出到文件
              StaticSpots.dat，自动识别jpg、png、bmp等格式；
            2.动态视频的光点实时探测，准确探测每一帧中光点的数量、亮度和近似半径，并输出到
              文件 DynamicSpots.dat，自动识别avi、mp4等格式；
            3.实时录像光点探测，准确探测摄像头实时采集的每一帧中光点的数量、中心亮度、坐标
              和近似半径，并将每一时刻的结果输出到文件 RealtimeSpots.dat    
作者: 邓裕强
版本: 1.0
--------------------------------------------------------------------------------
备注:-
--------------------------------------------------------------------------------
修改记录:
日  期        版 本    修 改 人    修改内容
2016/01/20    1.0      邓裕强      创建
</PRE>
********************************************************************************

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat blur_dst; Mat lap_dst; Mat diff; 
Mat dilation_dst; Mat erosion_dst; Mat mordst; Mat thredst;
int dilation_size = 2;
int max_size = 31;
int moph_size=1;
void Dilate(int, void*);

/** @function main */
int main( int argc, char** argv )
{
  vector<Vec3f> circles;
  /// 加载源图像
  src = imread( argv[1], 1 );

  /// 转成灰度并模糊化降噪
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// 创建窗体
  
  GaussianBlur(src_gray, blur_dst, Size(3,3), 0, 0);
  
  Laplacian(blur_dst, lap_dst, CV_8U, 31, 1, 0, BORDER_DEFAULT );

  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );
 
  GaussianBlur(lap_dst, blur_dst, Size(15,15), 0, 0);
  GaussianBlur(blur_dst, blur_dst, Size(15,15), 0, 0);

  threshold(blur_dst, thredst, 25, 300, 0);
  
  //absdiff(lap_dst, blur_dst, diff);
 
  

 // Mat element = getStructuringElement(2,Size(2*moph_size+1,2*moph_size+1),Point(    moph_size,moph_size));
  
  //morphologyEx(blur_dst, mordst, MORPH_GRADIENT, element);
 
  //blur(dilation_dst, blur_dst, Size(11,11));
  
  Laplacian( thredst, lap_dst, CV_8U, 19, 1, 0, BORDER_DEFAULT );
  
  Mat element = getStructuringElement( 2,
                                      Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );
  //createTrackbar ("lap", "Source", &dilation_size, max_size, Dilate);
  
  
  dilate( lap_dst, dilation_dst, element );
  
  
  element = getStructuringElement(2,Size(2*moph_size+1,2*moph_size+1),Point(    moph_size,moph_size));
  
  morphologyEx(dilation_dst, mordst, MORPH_GRADIENT, element);
  
  
  HoughCircles(mordst, circles, CV_HOUGH_GRADIENT, 2, 10, 100, 10, 0,10);
  for (size_t j=0; j<circles.size();j++) {
    Point center(cvRound(circles[j][0]), cvRound(circles[j][1]));
    int radius = cvRound(circles[j][2]);
   // circle(frame, center, 20, Scalar(255,255,0),-1,8,0);
    circle(src, center, radius, Scalar(100,255,100), 3, 8, 0);
  }
  namedWindow("dst", CV_WINDOW_AUTOSIZE);
  imshow("dst", src);
  waitKey(0);
  return(0);
}
