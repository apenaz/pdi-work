#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
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
    imshow("Resized image", resized);



    waitKey(0);

    return 0;
}

