#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat image;
String image_name;
String file;
String file_mmq;
vector<Point2i> pointList;
int radius = 8;
bool save = false;
ofstream outFile;

void mmq();
void CallBackFunc(int event, int x, int y, int flags, void *image_name);

int main(int argc, char **argv)
{
     time_t start = std::time(0); // get time now
     cout << "Started at:" << endl;
     tm *now = std::localtime(&start);
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
               cout << "reset image" << endl;
               image = imread(image_name, 4);
               pointList.clear();
          }
          else if (k == 's')
          {
               printf("Saving \n");
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

     else if (event == EVENT_LBUTTONDBLCLK)
     {
          save = false;
          outFile.open(file, std::ios_base::app);
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
          pointList.clear();
     }
     else if (event == EVENT_RBUTTONDOWN)
     {
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
                    circle(image, Point(x, y), radius, Scalar(0, 255, 0), 100, 8, 0);
               }
          }
     }
}

void mmq()
{
     cout << "mmq function" << endl;
     double a0 = 0;
     double a1 = 0;
     int sx = 0;
     int sy = 0;
     int sxy = 0;
     int sx2 = 0;
     uint16_t n = pointList.size();
     cout << "number of point: " << n << endl;
     if (n > 0)
     {
          for (uint16_t i = 0; i < pointList.size(); n++)
          {
               Point myPoint = pointList[i];
               sx = sx + myPoint.x;
               sy = sy + myPoint.y;
               sx2 = sx2 + myPoint.x * myPoint.x;
               sxy = sxy + myPoint.x * myPoint.y;
          }
          a0 = (sy * sx2 - sx * sxy) / (n * sx2 - sx * sx);
          a1 = (n * sxy - sx * sy) / (n * sx2 - sx * sx);
          outFile.open(file_mmq, std::ios_base::app);
          if (!outFile)
          {
               cout << "erro" << file << endl;
               abort();
          }
          Point2d myPoint = Point2d(a0, a1);
          outFile << myPoint;
          outFile << endl;
          outFile.close();
     }

     cout << " coeficientes a0 e a1: " << a0 << ", " << a1 << endl;
     pointList.clear();
}
