#ifndef _CHANEL_H_
#define _CHANEL_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
//using namespace std;
using namespace cv;

class Chanel:public Mat
{
private:
  int color; //0 rouge, 1 vert, 2 blue
public:
  Chanel();
  Chanel(Mat image, int c);
  Chanel(Chanel& chanel);
  Chanel& operator=(const Chanel& chanel);
  void drawProfil(Mat image, Point start, Point end);
  void drawProfil_simple(Mat image, Point start, Point end);
  void drawHist(Mat hist, int color);
  friend Scalar getScalar(Chanel);
  friend void initHistData(int data[], int N);
  void setHistData(int data[], int N);
  friend int getMax(int data[], int N);
  friend int getMin(int data[], int N);
  //  friend float lut(int i);
  Mat contrastLinaire();
  Mat contrastLinaireSaturation(int s_min, int s_max);
  Mat contrastLinaire2Point(int s_min, int s_max);
  Mat contrastLinaireMorceaux(float r1, float s1, float r2, float s2); 
  friend int calcGama(int gama);
  Mat contrastGama(float gama);
  void normalisationHist(float hn[], int h[], int N);
  friend void densite(float c[], float hn[], int N);
  Mat egalisationHist();
  
  int getColor();
  virtual ~Chanel();
};

#endif /* _CHANEL_H_ */














