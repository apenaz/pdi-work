import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

from skimage import io, feature, color, filters, img_as_float
count = 0
points = []

def draw_line(event,x,y,flags,param):
    global points, count
    if event == cv.EVENT_LBUTTONDOWN:
        points.append(x,y)
        count = count + 1
        if(count >= count%2):
            cv.line(tozero,points[0],points[1],(255,0,0),5)
            count = 0
    if event == cv.EVENT_LBUTTONDOWN:
        drawing = True
        ix,iy = x,y

    elif event == cv.EVENT_MOUSEMOVE:
        if drawing == True:
            if mode == True:
                cv.rectangle(img,(ix,iy),(x,y),(0,255,0),-1)
            else:
                cv.circle(img,(x,y),5,(0,0,255),-1)

    elif event == cv.EVENT_LBUTTONUP:
        drawing = False
        if mode == True:
            cv.rectangle(img,(ix,iy),(x,y),(0,255,0),-1)
        else:
            cv.circle(img,(x,y),5,(0,0,255),-1)

def plot_treatement(i):
    image_to_read = 'png/img' + str(i) + '.png'
    print(image_to_read)
    img = cv.imread(image_to_read,0)
    canny_img = cv.Canny(img,80,200)

    ret,tozero = cv.threshold(img,170,255,cv.THRESH_TOZERO)
    canny_tozero = cv.Canny(tozero,80,200)

    median_blur = cv.medianBlur(tozero,5)
    canny_median_blur = cv.Canny(median_blur,80,200)

    adaptative_gaussian = cv.adaptiveThreshold(tozero,255,\
        cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv.THRESH_BINARY,11,2)
    canny_adaptative_gaussian = cv.Canny(adaptative_gaussian,80,200)


    adaptative_mean = cv.adaptiveThreshold(tozero,255,\
        cv.ADAPTIVE_THRESH_MEAN_C,\
            cv.THRESH_BINARY,11,2)
    canny_adaptative_mean = cv.Canny(adaptative_mean,80,200)


    ret,binary = cv.threshold(tozero,127,255,cv.THRESH_BINARY)
    canny_binary = cv.Canny(img,80,200)




    titles = ['Original Image','TOZERO','median_blur',\
        'ADAPTIVE_THRESH_GAUSSIAN_C','adaptative_mean',\
            'binary threshold','.','..','.','..','.','..' ]

    images = [img, tozero, median_blur, adaptative_gaussian,\
        adaptative_mean, binary,\
            canny_img, canny_tozero, canny_median_blur,\
                canny_adaptative_gaussian,\
        canny_adaptative_mean, canny_binary]
    events = [i for i in dir(cv) if 'EVENT' in i]

    for i in range(12):
        plt.subplot(2,6,i+1),plt.imshow(images[i],'gray')
        plt.title(titles[i])
        plt.xticks([]),plt.yticks([])
    plt.show()

for i in range(16):
    plot_treatement(i)

while(1):
    img = np.zeros((512,512,3), np.uint8)
    cv.namedWindow('image')
    cv.setMouseCallback('image',draw_line)

    cv.imshow('image',tozero)
    if cv.waitKey(20) & 0xFF == 27:
        break
cv.destroyAllWindows()
'''
if __name__ == "__main__":
    main()
'''

'''
# Find the contours
#imgray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
edges = cv.Canny(img,60,200)
im2, contours, hierarchy = cv.findContours(edges, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

hierarchy = hierarchy[0] # get the actual inner list of hierarchy descriptions

# For each contour, find the bounding rectangle and draw it
cv.drawContours(img, contours, -1, (0,255,0), 3)

# Finally show the image
cv.imshow('img',img)
cv.waitKey(0)
cv.destroyAllWindows()
'''
'''
img = cv.imread('C-Curve.png',0)
img = cv.medianBlur(img,5)
ret,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
th2 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_MEAN_C,\
            cv.THRESH_BINARY,11,2)
th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv.THRESH_BINARY,11,2)
titles = ['Original Image', 'Global Thresholding (v = 127)',
            'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
images = [img, th1, th2, th3]
for i in range(4):
    plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
plt.show()
'''
'''
img = cv.imread('C-Curve.png',0)
# global thresholding
ret1,th1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
# Otsu's thresholding
ret2,th2 = cv.threshold(img,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
# Otsu's thresholding after Gaussian filtering
blur = cv.GaussianBlur(img,(5,5),0)
ret3,th3 = cv.threshold(blur,0,255,cv.THRESH_BINARY+cv.THRESH_OTSU)
# plot all the images and their histograms
images = [img, 0, th1,
          img, 0, th2,
          blur, 0, th3]
titles = ['Original Noisy Image','Histogram','Global Thresholding (v=127)',
          'Original Noisy Image','Histogram',"Otsu's Thresholding",
          'Gaussian filtered Image','Histogram',"Otsu's Thresholding"]
for i in range(3):
    plt.subplot(3,3,i*3+1),plt.imshow(images[i*3],'gray')
    plt.title(titles[i*3]), plt.xticks([]), plt.yticks([])
    plt.subplot(3,3,i*3+2),plt.hist(images[i*3].ravel(),256)
    plt.title(titles[i*3+1]), plt.xticks([]), plt.yticks([])
    plt.subplot(3,3,i*3+3),plt.imshow(images[i*3+2],'gray')
    plt.title(titles[i*3+2]), plt.xticks([]), plt.yticks([])
plt.show()
'''