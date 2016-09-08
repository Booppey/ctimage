#include <iostream>
#include <stdio.h>
#include <vector>
#include "chanel.h"
#include "control.h"

using namespace std;
using namespace cv;

Control::Control()
{
}

Control::Control(Mat img)
{
  image = img;
}

Control::Control(Mat img, Point start, Point end)
{
  image = img;
  line_profil_start = start;
  line_profil_end = end;
}

Mat Control::getImage()
{
  return image;
}

void Control::input(string file_name)
{
  image = imread(file_name, 1);
}

bool Control::output(string file_name)
{
  bool res = imwrite(file_name, image);
  return res;
}

void Control::drawLine()
{
    line(image, line_profil_start,
	 line_profil_end, Scalar( 255, 0, 0), 1, 8, 0);
}

void Control::setStartPoint(Point point)
{
  line_profil_start = point;
}

void Control::setEndPoint(Point point)
{
  line_profil_end = point;
}

void Control::drawProfil(int option, string image_name,
			 int x1, int y1, int x2, int y2)
{
  Mat image;
  this->input(image_name);
  image = this->getImage();
  Point start(x1, y1);
  Point end(x2, y2);
  this->setStartPoint(start);
  this->setEndPoint(end);
  float lengh = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
  Mat profil_img(260, lengh + 10, CV_8UC3, Scalar(255, 255, 255));
  Mat hist(600, 256, CV_8UC3, Scalar(255, 255, 255));
  if (option == 0)
    {
      cvtColor(image, image, CV_RGB2GRAY);
      Chanel chanel(image, 0);
      chanel.drawProfil(profil_img, start, end);
      chanel.drawHist(hist, 1);
    }
  if (option == 1)
    {
      vector<Mat> rgb_planes;
      Chanel chanel;
      split(image, rgb_planes );
      for (int i = 0; i < 3; i++)
	{
	  Chanel chanel(rgb_planes[i], i);
	  chanel.drawProfil(profil_img, start, end);
	  chanel.drawHist(hist, i+1);
	}
    }
  this->drawLine();
  this->output("image/image_out.tif");
  imwrite("image/profil.tif", profil_img);
  imwrite("image/hist.tif", hist);
}

void Control::calcContrast(int option, string image_name,
			   int function)
{
  Mat image;
  this->input(image_name);
  image = this->getImage();
  Mat image_contrast = image.clone();
  Mat hist(600, 256, CV_8UC3, Scalar(255, 255, 255));
  Mat hist_contrast(600, 256, CV_8UC3, Scalar(255, 255, 255));
  int s_min, s_max;
  int r1, s1, r2, s2;
  float gama;
  if (function == 2)
    {
      cout<<"Entrez: Smin Smax\n";
      cin>>s_min >>s_max;
    }
  if (function == 3)
    {
      cout<<"Entrez 2 points: r1 s1 r2 s2\n";
      cin>>r1 >>s1 >>r2 >>s2;
    }
  if (function == 4)
    {
      cout<<"Entrez gama: gama\n";
      cin>>gama;
    }
  if (option == 0)
    {
      cvtColor(image, image, CV_RGB2GRAY);
      Chanel chanel(image, 0);
      chanel.drawHist(hist, 1);
      switch(function)
	{
	case 1:
	  image_contrast = chanel.contrastLinaire();
	  break;
	case 2:
	  image_contrast = chanel.contrastLinaireSaturation(s_min, s_max);
	  break;
	case 3:
	  image_contrast = chanel.contrastLinaireMorceaux((float)r1, (float)s1, (float)r2, (float)s2);
	  break;
	case 4:
	  image_contrast = chanel.contrastGama(gama);
	  break;
	case 5:
	  image_contrast = chanel.egalisationHist();
	  break;

	}
      Chanel chanel_contrast(image_contrast, 0);
      chanel.drawHist(hist_contrast, 1);
    }
  if (option == 1)
    {
      vector<Mat> rgb_planes;
      split(image, rgb_planes);
      for (int i = 0; i < 3; i++)
	{
	  Chanel chanel(rgb_planes.at(i), i);
	  chanel.drawHist(hist, i+1);
	  switch(function)
	    {
	    case 1:
	      image_contrast = chanel.contrastLinaire();
	      break;
	    case 2:
	      image_contrast = chanel.contrastLinaireSaturation(s_min, s_max);
	      break;
	    case 3:
	      image_contrast = chanel.contrastLinaireMorceaux((float)r1, (float)s1, (float)r2, (float)s2);
	      break;
	    case 4:
	      image_contrast = chanel.contrastGama(gama);
	      break;
	    case 5:
	      image_contrast = chanel.egalisationHist();
	      break;
	    }
	  Chanel chanel_contrast(rgb_planes.at(i), i);
	  chanel.drawHist(hist_contrast, i+1);
	}
      merge(rgb_planes, image_contrast);
    }
  imwrite("image/hist.tif", hist);
  imwrite("image/contrast.tif", image_contrast);
  imwrite("image/hist_contrast.tif", hist_contrast);
}












