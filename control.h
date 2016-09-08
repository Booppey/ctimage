#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace std;
using namespace cv;

class Control
{
 private:
  Mat image;
  Point line_profil_start;
  Point line_profil_end;
 public:
  Control();
  Control(Mat img);
  Control(Mat img, Point start, Point end);
  // virtual ~Control();
  void input(string file_name);
  bool output(string file_name);
  void setStartPoint(Point point);
  void setEndPoint(Point point);
  void drawProfil(int option, string image_name,
		  int x1, int y1, int x2, int y2);
  void calcContrast(int option, string image_name,
		    int function);
  Mat getImage();
  void drawLine();
};

#endif /* _CONTROL_H_ */














