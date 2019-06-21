from __future__ import print_function
from __future__ import division
import logging
import cv2 as cv
import tkinter as tk
import numpy as np
from tkinter import filedialog # para interface abrir arquivo
from matplotlib import pyplot as plt # exibir subimagens
import argparse

'''
Read image and convert it to binary
(usado threshold tozero)

Extract objects of interest

Extract orientation

Visualize result
'''
# globais 
count = 0
points = []

val_median = 5

janela_controles = 'Controles'
janela_imagem_threshold = 'bin'
janela_imagem_mediana = 'median'
janela_sobel = 'sobel'
janela_filtro_canny = 'Canny'

'''
funções
'''
def nothing(x):
      pass
def draw_line(event, x, y, flags, param):
    global points, count
    if event == cv.EVENT_LBUTTONDOWN:
        points.append(x, y)
        count = count + 1
        if(count >= count % 2):
            cv.line(tozero, points[0], points[1], (255, 0, 0), 5)
            count = 0
    if event == cv.EVENT_LBUTTONDOWN:
        drawing = True
        ix, iy = x, y

    elif event == cv.EVENT_MOUSEMOVE:
        if drawing == True:
            if mode == True:
                cv.rectangle(img, (ix, iy), (x, y), (0, 255, 0), -1)
            else:
                cv.circle(img, (x, y), 5, (0, 0, 255), -1)

    elif event == cv.EVENT_LBUTTONUP:
        drawing = False
        if mode == True:
            cv.rectangle(img, (ix, iy), (x, y), (0, 255, 0), -1)
        else:
            cv.circle(img, (x, y), 5, (0, 0, 255), -1)

'''
janelas  e controles
usados no programa
'''
#janelas
cv.namedWindow(janela_controles, cv.WINDOW_NORMAL)
cv.namedWindow(janela_filtro_canny, cv.WINDOW_NORMAL)

# controles
barra_threshold_binary = 'threshold binary'
barra_canny_min = 'min val'
barra_canny_max = 'max val'
barra_median = "mediana val"

cv.createTrackbar(barra_threshold_binary, janela_controles,0,255,nothing)
cv.createTrackbar(barra_canny_min, janela_controles,0,255,nothing)
cv.createTrackbar(barra_canny_max, janela_controles,0,255,nothing)
cv.createTrackbar(barra_median, janela_controles,1,25,nothing)

# Homomorphic filter class
class HomomorphicFilter:
    """Homomorphic filter implemented with diferents filters and an option to an external filter.
    
    High-frequency filters implemented:
        butterworth
        gaussian
    Attributes:
        a, b: Floats used on emphasis filter:
            H = a + b*H
        
        .
    """

    def __init__(self, a = 0.5, b = 1.5):
        self.a = float(a)
        self.b = float(b)

    # Filters
    def __butterworth_filter(self, I_shape, filter_params):
        P = I_shape[0]/2
        Q = I_shape[1]/2
        U, V = np.meshgrid(range(I_shape[0]), range(I_shape[1]), sparse=False, indexing='ij')
        Duv = (((U-P)**2+(V-Q)**2)).astype(float)
        H = 1/(1+(Duv/filter_params[0]**2)**filter_params[1])
        return (1 - H)

    def __gaussian_filter(self, I_shape, filter_params):
        P = I_shape[0]/2
        Q = I_shape[1]/2
        H = np.zeros(I_shape)
        U, V = np.meshgrid(range(I_shape[0]), range(I_shape[1]), sparse=False, indexing='ij')
        Duv = (((U-P)**2+(V-Q)**2)).astype(float)
        H = np.exp((-Duv/(2*(filter_params[0])**2)))
        return (1 - H)

    # Methods
    def __apply_filter(self, I, H):
        H = np.fft.fftshift(H)
        I_filtered = (self.a + self.b*H)*I
        return I_filtered

    def filter(self, I, filter_params, filter='butterworth', H = None):
        """
        Method to apply homormophic filter on an image
        Attributes:
            I: Single channel image
            filter_params: Parameters to be used on filters:
                butterworth:
                    filter_params[0]: Cutoff frequency 
                    filter_params[1]: Order of filter
                gaussian:
                    filter_params[0]: Cutoff frequency
            filter: Choose of the filter, options:
                butterworth
                gaussian
                external
            H: Used to pass external filter
        """

        #  Validating image
        if len(I.shape) is not 2:
            raise Exception('Improper image')

        # Take the image to log domain and then to frequency domain 
        I_log = np.log1p(np.array(I, dtype="float"))
        I_fft = np.fft.fft2(I_log)

        # Filters
        if filter=='butterworth':
            H = self.__butterworth_filter(I_shape = I_fft.shape, filter_params = filter_params)
        elif filter=='gaussian':
            H = self.__gaussian_filter(I_shape = I_fft.shape, filter_params = filter_params)
        elif filter=='external':
            print('external')
            if len(H.shape) is not 2:
                raise Exception('Invalid external filter')
        else:
            raise Exception('Selected filter not implemented')
        
        # Apply filter on frequency domain then take the image back to spatial domain
        I_fft_filt = self.__apply_filter(I = I_fft, H = H)
        I_filt = np.fft.ifft2(I_fft_filt)
        I = np.exp(np.real(I_filt))-1
        return np.uint8(I)
# End of class HomomorphicFilter

'''
leitura da imagem
abre uma janela para escolha da imagem
'''
root = tk.Tk()
root.withdraw()
file_path = filedialog.askopenfilename()
print(file_path)
src = cv.imread(file_path,0)

''' 
janela para exibição da imagem com a possibilidade de redimencionamento
'''

while (1):
    # ler valores dos controles
    val_thr_bin = cv.getTrackbarPos(barra_threshold_binary, janela_controles)
    val_canny_min = cv.getTrackbarPos(barra_canny_min, janela_controles)
    val_canny_max = cv.getTrackbarPos(barra_canny_max, janela_controles)
    
    # https://docs.opencv.org/master/d7/d4d/tutorial_py_thresholding.html
    ret,tozero = cv.threshold(src,val_thr_bin,255,cv.THRESH_TOZERO)
    
    # estava muito ruim aplicar as bordas
    #equalização do histogram e suavização       
    equalizado = cv.equalizeHist(tozero,255)
    blur = cv.blur(src,(3,3))
    median = cv.medianBlur(blur,3)
    median = cv.medianBlur(median,3)
    median = cv.medianBlur(median,3)
    
    
    #bordas 
    # https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_gradients/py_gradients.html
    homo_filter = HomomorphicFilter(a = 0.75, b = 1.25)
    img_filtered = homo_filter.filter(I=median, filter_params=[30,2])
    median = cv.medianBlur(img_filtered,3)
    sobelx64f = cv.Sobel(median,cv.CV_64F,1,1,ksize=5)
    abs_sobel64f = np.absolute(sobelx64f)
    sobel_8u = np.uint8(abs_sobel64f)
    median = cv.medianBlur(sobel_8u,3)
    #https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_canny/py_canny.html
    bordas = cv.Canny(median,val_canny_min,val_canny_max)
    #colocar condicional para escolher aplicar filtro na original ou na binária
 
    # ao final do programa salva  a imagem se for digitado s
    # ou sai sem salvar se for digitado ESC
    # & 0xFF (adicionado em sistemas 64 bits)
    
    #cv.imshow(janela_imagem_threshold,tozero)
    #cv.imshow(janela_imagem_mediana,median)
    #cv.imshow(janela_sobel ,sobel_8u)
    
    cv.imshow(janela_filtro_canny,bordas)
    ## find contornos

    k = cv.waitKey(10)  & 0xFF
    if k == 27:         
        cv.destroyAllWindows()
        break
    elif k == ord('s'): 
        cv.imwrite('saida.png',src)
        cv.destroyAllWindows()
        break
