#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>



using namespace cv;
using namespace std;


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
          cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_RBUTTONDOWN )
     {
          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if  ( event == EVENT_MBUTTONDOWN )
     {
          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
     }
     else if ( event == EVENT_MOUSEMOVE )
     {
          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

     }
}



int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image, resized;
    image = imread( argv[1], 1 );

    double fator = 720.0 / image.rows;
    cout<<"fator: "<< fator << endl ;

    resize(image, resized, Size(image.cols * fator, image.rows * fator), 0, 0);

    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Original Image", WINDOW_AUTOSIZE );
    imshow("Original Image", image);

    namedWindow("Resized image", WINDOW_AUTOSIZE);
    setMouseCallback("Resized image", CallBackFunc, NULL);
    imshow("Resized image", resized);



    waitKey(0);

    return 0;
}

