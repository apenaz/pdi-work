#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


bool save = false;
bool call_mmq = false;
vector<Point2i> pointList;

void CallBackFunc(int event, int x, int y, int flags, void *image_name)
{
     if (event == EVENT_LBUTTONDOWN)
     {
          save = true;
          std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if (event == EVENT_LBUTTONUP)
     {
          setMouseCallback((char*)image_name, NULL, NULL);
          save = false;
          call_mmq = true;
          std::cout << "call_mmq: " << call_mmq << endl;
          std::cout << "Left button of the mouse is realese - position (" << x << ", " << y << ")" << endl;
     }
     else if (event == EVENT_RBUTTONDOWN)
     {
          std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if (event == EVENT_MBUTTONDOWN)
     {
          std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if (event == EVENT_MOUSEMOVE)
     {
          if (save)
          {
               std::cout << "SAVED - position (" << x << ", " << y << ")" << endl;
               pointList.push_back(Point(x,y));
          }
          else
          {
               std::cout << "position (" << x << ", " << y << ")"
                    << "NOT SAVED" << endl;
          }
     }
}

void mmq(const String window_name)
{
     std::cout << "mmq function" << endl;
     if (call_mmq)
     {
          for (uint64_t n = 0; n < pointList.size(); n++)
          {
               Point myPoint = pointList[n];
               std::cout << myPoint << "..";
          }
          std::cout << endl;
     }
     call_mmq = false;
     std::cout << "END mmq() call_mmq: " << call_mmq << endl;
     
     setMouseCallback(window_name, CallBackFunc, NULL);
}

int main(int argc, char **argv)
{
     const String image_name = argv[1];
     Mat image;

     if (argc != 2)
     {
          printf("usage: DisplayImage.out <Image_Path>\n");
          return -1;
     }
     image = imread(image_name, 4);
     if (!image.data)
     {
          printf("No image data \n");
          return -1;
     }
     char k;
     namedWindow(image_name, WINDOW_KEEPRATIO);
     setMouseCallback(image_name, CallBackFunc, NULL);
     while (1)
     {
          namedWindow(image_name, WINDOW_KEEPRATIO);
          imshow(image_name, image);

          if (call_mmq)
          {
               mmq(image_name);
          }

          k = waitKey(0);
          if (k == 27)
          {
               destroyAllWindows();
               break;
          }
          else if (k == 'r')
          {
               //reiniciar o vetor de pontos...
          }
          else if (k == 's')
          {
               imwrite("saida.png", image);
               destroyAllWindows();
               break;
          }
     }
     return 0;
}