#include <cmath>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
Mat image;
String image_name;
String file;
String file_mmq;
vector<Point2i> pointList;
vector<double_t> m_vector;
double radius = 0.01;
bool save = false;
ofstream outFile;
void mmq();
void CallBackFunc(int event, int x, int y, int flags, void *image_name);

int main(int argc, char **argv)
{
     time_t start = time(0); // get time now
     cout << "Started at:" << endl;
     tm *now = localtime(&start);
     int y = (now->tm_year + 1900),
         m = (now->tm_mon + 1),
         d = now->tm_mday,
         h = now->tm_hour,
         min = now->tm_min;
     char datec[13];
     snprintf(datec, 13, "%d-%d-%d-%d-%d", y, m, d, h, min);
     String date = datec;

     cout << date << endl;

     image_name = argv[1]; /* C:/path/ */
     file = image_name + date + ".pts";
     file_mmq = image_name + date + ".mmq";
     char k;
     if (argc != 2)
     {
          cout << "usage: DisplayImage.out <Image_Path>" << endl;
          return -1;
     }
     image = imread(image_name, 4);
     radius *= image.rows;
     if (!image.data)
     {
          cout << "No image data" << endl;
          return -1;
     }
     while (1)
     {
          namedWindow(image_name, WINDOW_KEEPRATIO);
          setMouseCallback(image_name, CallBackFunc, NULL);
          imshow(image_name, image);

          k = waitKey(30);
          if (k == 27)
          {
               cout << "Stop app" << endl;
               destroyAllWindows();
               break;
          }
          else if (k == 'r')
          {
               //restart pointList
               cout << "wait reset image" << endl;
               image = imread(image_name, 4);
               pointList.clear();
               m_vector.clear();
          }
          else if (k == 's')
          {
               cout << "Saving" << endl;
               String outputimage = image_name + date + "out.png";
               imwrite(outputimage, image);
               cout << "Saved" << endl;
          }
     }
     return 0;
}

void CallBackFunc(int event, int x, int y, int flags, void *image_name)
{
     if (event == EVENT_LBUTTONDOWN)
     {
          save = true;
     }
     else if (event == EVENT_LBUTTONUP)
     {
          save = false;
     }
     else if (event == EVENT_RBUTTONDOWN)
     {
          mmq();
     }
     else if (event == EVENT_LBUTTONDBLCLK)
     {
          save = false;
          outFile.open(file, ios_base::app);
          if (!outFile)
          {
               cout << "erro" << file << endl;
               abort();
          }
          for (uint16_t n = 0; n < pointList.size(); n++)
          {
               Point myPoint = pointList[n];
               outFile << myPoint << " ";
          }
          outFile << endl;
          outFile.close();
          mmq();
     }
     else if (event == EVENT_MOUSEMOVE)
     {
          if (save)
          {
               if (true /* não salvar se o valor dos pontos for além da imagem */)
               {
                    pointList.push_back(Point(x, y));
                    /*circle(image,                  //Image where the circle is drawn.
                      Point(x, y),            //Center of the circle.
                      radius,                 //Radius of the circle.
                      Scalar(255, 0, 0, 255), //Circle color.
                      1,                      //Thickness of the circle outline, if positive. Negative thickness means that a filled circle is to be drawn.
                      2,                      //Type of the circle boundary. See the line() description.
                      0);                     //Number of fractional bits in the coordinates of the center and in the radius value.
                      */
                    circle(image, Point(x, y), radius, Scalar(0, 255, 0), radius*0.5, 8, 0);
               }
          }
     }
}

void mmq()
{
     double a0 = 0;
     double a1 = 0;
     double x, y;
     double sx = 0;
     double sy = 0;
     double sxy = 0;
     double sx2 = 0;
     uint16_t n = pointList.size();
     if (n > 0)
     {
          for (uint16_t i = 0; i < n; i++)
          {
               Point myPoint = pointList[i];
               x = myPoint.x;
               y = myPoint.y;
               sx = sx + x;
               sy = sy + y;
               sx2 = sx2 + x * x;
               sxy = sxy + x * y;
          }
          a0 = (sy * sx2 - sx * sxy) / (n * sx2 - sx * sx);
          a1 = (n * sxy - sx * sy) / (n * sx2 - sx * sx);
          m_vector.push_back(a1);
          outFile.open(file_mmq, ios_base::app);
          if (!outFile)
          {
               cout << "erro" << file << endl;
               abort();
          }
          Point2d myPoint = Point2d(a0, a1);
          outFile << myPoint;
          outFile << endl;
     }
     int total_m = m_vector.size();
     while (total_m > 1)
     {
          if (total_m % 2 == 0)
          {
               double angle = atan(abs((m_vector[total_m - 1] - m_vector[total_m - 2]) / (1 + m_vector[total_m - 1] * m_vector[total_m - 2])));
               angle = (180.0 / M_PI) * angle;
               cout << "cobb angle: " << angle << endl;
               outFile << "cobb angle: " << angle << endl;
               total_m -= 2;
          }
          else
          {
               total_m--;
          }
     }
     int x0 = 1;
     int y0 = a0 + a1 * x0;
     int xn = image.cols - 1;
     int yn = a0 + a1 * xn;
     line(image, Point(x0, y0), Point(xn, yn), Scalar(0, 0, 250), radius*0.5, 8, 0);
     pointList.clear();
     outFile.close();
}
