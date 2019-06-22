#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

String image_name;
bool save = false;
vector<Point2i> pointList;
ofstream outFile;

void mmq();
void CallBackFunc(int event, int x, int y, int flags, void *image_name);

void CallBackFunc(int event, int x, int y, int flags, void *image_name)
{
     if (event == EVENT_LBUTTONDOWN)
     {
          save = true;
     }
     else if (event == EVENT_LBUTTONUP)
     {
          save = false;
          mmq();
     }
     else if (event == EVENT_MOUSEMOVE)
     {
          if (save)
          {
               pointList.push_back(Point(x, y));
          }
     }
}

void mmq()
{
     std::cout << "mmq function" << endl;
     String file = image_name + ".abj";
     outFile.open(file , std::ios_base::app);
     if (! outFile) 
    {
        cout << "erro" << file << endl; 
        abort(); 
    } 
     for (uint64_t n = 0; n < pointList.size(); n++)
     { //calcula os coeficientes a e b;
          Point myPoint = pointList[n];
          outFile << myPoint << " ";
          std::cout << myPoint << " ";
     }
     std::cout << endl;
     outFile << endl;
     outFile.close();
}

int main(int argc, char **argv)
{
     image_name = argv[1]; /* C:/path/ */
     Mat image;
     char k;
     namedWindow(image_name, WINDOW_KEEPRATIO);
     setMouseCallback(image_name, CallBackFunc, NULL);

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
     while (1)
     {
          namedWindow(image_name, WINDOW_KEEPRATIO);
          imshow(image_name, image);

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