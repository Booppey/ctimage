#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "chanel.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
using namespace std;

#define SUM_LEVEL 256

Chanel::Chanel()
  :Mat()
{
  color = 0;
}

Chanel::Chanel(Mat image, int c)
  :Mat(image)
{
  color = c;
}

Chanel::Chanel(Chanel& chanel)
  :Mat((Mat&)chanel)
{
  color = chanel.color;
}

Chanel& Chanel::operator=(const Chanel& chanel)
{
  Mat::operator=(chanel);
  color = chanel.color;
  return *this;
}

Chanel::~Chanel()
{
}

Scalar getScalar(Chanel chanel)
{
    switch(chanel.color)
    {
    case 0:
      return Scalar(255, 0, 0);
    case 1:
      return Scalar(0, 255, 0);
    case 2:
      return Scalar(0, 0, 255);
    }
    return  Scalar(255, 0, 0);
}  

void Chanel::drawProfil(Mat profil_img, Point start, Point end)
{
  float x_start, y_start;
  float x_end, y_end;
  float col, row;
  uchar data_start;
  uchar data_end;
  float dx, dy;
  float sin_line, cos_line;
  Point point_start, point_end;
  Scalar scalar;
  float lengh;
  scalar = getScalar(*this);
  x_start = start.x;
  y_start = start.y;
  x_end = end.x;
  y_end = end.y;
  dx = x_end - x_start;
  dy = y_end - y_start;
  lengh = sqrt(dx*dx + dy*dy);
  sin_line = dy/lengh;
  cos_line = dx/lengh;
  col = x_start;
  row = y_start;
  for( int i = 1; i < lengh; i++ )
    {
      data_start = this->at<uchar>(row, col);
      row = y_start + sin_line;
      col = x_start + cos_line;
      data_end = this->at<uchar>(row, col);
      point_start = Point(i, 255-data_start);
      point_end = Point(i-1, 255-data_end);
      line( profil_img, point_start, point_end, scalar, 1, 8, 0 );
      y_start = row;
      x_start = col;
    }
}


void Chanel::drawProfil_simple(Mat profil_img, Point start, Point end)
{
  uchar r;
  uchar r0;
  int img_w = this->cols;
  int cen_img = this->rows/2+5;
  cout<<img_w;
  ///Dessigner le profil de la ligne
  for( int i = 1; i < img_w; i++ )
    {
      r = this->at<uchar>(cen_img, i);
      r0 = this->at<uchar>(cen_img, i-1);
      
      line( profil_img, Point(i, 255-r), Point(i-1, 255-r0),
	    Scalar( 0, 0, 255), 1, 8, 0 ); 
    }

}

void initHistData(int data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void initHistData(float data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void initHistData(uchar data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void Chanel::setHistData(int data[], int N)
{
  int data_index = 0;
  int img_w = this->cols;
  int img_h = this->rows;
  for(int i = 0; i < img_h; i++)
    for (int j = 0; j < img_w; ++j)
      {
	data_index = this->at<uchar>(i,j);
	data[data_index]++;
      }
}

int getMaxArr(int data[], int N)
{
  int data_element = 0;
  int max = 0;
  for (int i = 0; i < N; ++i)
    {
      data_element = data[i];
      if (max < data_element)
	max = data_element;
    }
  return max;
}

int getMin(uchar **data, int M, int N)
{
  int min = 255;
  uchar data_element = 0;
  for (int i = 0; i < M; ++i)
    {
      for (int j = 0; j < N;  ++j)
	{
	  data_element = data[i][j];
	  if (min > data_element)
	    {
	      min = data_element;
	    }
	}
    }
  return min;
}

int getMax(uchar **data, int M, int N)
{
  int max = 0;
  uchar data_element = 0;
  for (int i = 0; i < M; ++i)
    {
      for (int j = 0; j < N;  ++j)
	{
	  data_element = data[i][j];
	  if (max < data_element)
	    {
	      max = data_element;
	    }
	}
    }
  return max;
}


void Chanel::drawHist(Mat hist, int color)
{
  int hist_h = hist.rows/3;

  int data[SUM_LEVEL];
  Scalar scalar;
  Point point_start, point_end;
  initHistData(data, SUM_LEVEL);
  setHistData(data, SUM_LEVEL);
  int max = getMaxArr(data, SUM_LEVEL);
  scalar = getScalar(*this);
  for (int i = 1; i < SUM_LEVEL; i++)
    {
      point_start = Point(i, color*hist_h);
      point_end = Point(i, color*hist_h - hist_h*data[i]/max);
      line( hist, point_start, point_end, scalar, 1, 8, 0 );
    }
}

Mat Chanel::contrastLinaire()
{
  int img_h = this->rows;
  int img_w = this->cols;
  int lut[SUM_LEVEL];
  uchar **I;
  I = new uchar*[img_h];
  for (int i = 0; i < img_h; ++i)
    I[i] = new uchar[img_w];

  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
       I[i][j] = this->at<uchar>(i, j);

  int _max = getMax(I, img_h, img_w);
  int _min = getMin(I, img_h, img_w);
  float a = 255/(_max - _min);
  for (int i = 0; i < SUM_LEVEL; ++i)
    lut[i] = (i - _min)*a;
  
  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      this->at<uchar>(i, j) = lut[I[i][j]];
  for (int i = 0; i < img_h; ++i)
    {
      delete[] I[i];
    }
  delete[] I;

  return *this;
}

Mat Chanel::contrastLinaireSaturation(int s_min, int s_max)
{
  int img_h = this->rows;
  int img_w = this->cols;
  int lut[SUM_LEVEL];
  uchar **I;
  I = new uchar*[img_h];
  for (int i = 0; i < img_h; ++i)
    I[i] = new uchar[img_w];

  int _max = s_max;
  int _min = s_min; 
  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      {
	I[i][j] = this->at<uchar>(i, j);
      }

  for (int i = 0; i <= SUM_LEVEL; ++i)
    lut[i] = 255*(i - _min)/(_max - _min);
  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      {
	if (lut[I[i][j]] < 0)
	  this->at<uchar>(i, j) = 0;
	else if(lut[I[i][j]] > 255)
	  this->at<uchar>(i, j) = 255;
	else this->at<uchar>(i, j) = lut[I[i][j]];
      }
  for (int i = 0; i < img_h; ++i)
    {
      delete[] I[i];
    }
  delete[] I;

  return *this;
}

Mat Chanel::contrastLinaireMorceaux(float r1, float s1, float r2, float s2)
{
  if(r2 == r1 || r1 == 0 || r2 == 255)
    return *this;
  int img_h = this->rows;
  int img_w = this->cols;
  uchar **I;
  I = new uchar*[img_h];
  for (int i = 0; i < img_h; ++i)
    I[i] = new uchar[img_w];

  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      {
	I[i][j] = this->at<uchar>(i, j);
      }

  int T[SUM_LEVEL];
  for (int r = 0; r < SUM_LEVEL; ++r)
    {
      if(r <= r1)
	T[r] = r*s1/r1;
      else if(r >= r2)
	T[r] = s2 +(r-r2)*(255-s2)/(255-r2);
      else
	T[r] = s1 + (r-r1)*(s2-s1)/(r2-r1);
    }

  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      {
	this->at<uchar>(i, j) = T[I[i][j]];
      }
  for (int i = 0; i < img_h; ++i)
    {
      delete[] I[i];
    }
  delete[] I;

  return *this;
}

int calcGama(int valeur, float gama)
{
  float vl = (float) valeur;
  float res = (float)pow(vl / 255.0, 1.0/gama);
  return (int) (255.0 * res);
}

Mat Chanel::contrastGama(float gama)
{
  int img_h = this->rows;
  int img_w = this->cols;
  int lut[SUM_LEVEL];
  uchar **I;
  I = new uchar*[img_h];
  for (int i = 0; i < img_h; ++i)
    I[i] = new uchar[img_w];

  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      {
	I[i][j] = this->at<uchar>(i, j);
      }

  for (int i = 0; i < SUM_LEVEL; ++i)
    lut[i] = calcGama(i, gama);

  for (int i = 0; i < img_h; ++i)
    for (int j = 0; j < img_w; ++j)
      if (lut[I[i][j]] < 0)
	this->at<uchar>(i, j) = 0;
      else if(lut[I[i][j]] > 255)
	this->at<uchar>(i, j) = 255;
      else this->at<uchar>(i, j) = lut[I[i][j]];

  for (int i = 0; i < img_h; ++i)
    delete[] I[i];
  delete[] I;

  return *this;
}

void Chanel::normalisationHist(float hn[], int h[], int N)
{
  int nbp = (this->cols)*(this->rows);
  for(int i = 0; i < N; i++)
    hn[i] = (float)h[i]/(float)nbp;
}

void densite(float c[], float hn[], int N)
{
  for(int i = 0; i < N; i++)
    {
      c[i] = 0.0;
      for(int j = 0; j <= i; j++)
	c[i] += hn[j];
    }
}

Mat Chanel::egalisationHist()
{
  int img_h = this->rows;
  int img_w = this->cols;
  int h[SUM_LEVEL];
  float hn[SUM_LEVEL];
  float c[SUM_LEVEL];
  initHistData(h, SUM_LEVEL);
  initHistData(hn, SUM_LEVEL);
  initHistData(c, SUM_LEVEL);
  setHistData(h, SUM_LEVEL);
  normalisationHist(hn, h, SUM_LEVEL);
  densite(c, hn, SUM_LEVEL);
  int index = 0;
  for (int i = 0; i < img_h; ++i)
    {
      for (int j = 0; j < img_w; ++j)
	{
	  index = this->at<uchar>(i, j);
	  this->at<uchar>(i, j) = c[index]*255;
	}
    }
  return *this;
}




