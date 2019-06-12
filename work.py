from __future__ import print_function
from __future__ import division
import cv2 as cv
import tkinter as tk
from tkinter import filedialog # para interface abrir arquivo
from matplotlib import pyplot as plt # exibir subimagens
import argparse

'''
barras
'''
def nothing(x):
      pass
# nome da janela de controles
janela_controles = 'Controles'
barra_threshold_binary = 'threshold binary'
barra_outra = 'Min'

cv.namedWindow(janela_controles)
cv.createTrackbar(barra_threshold_binary, janela_controles,0,255,nothing)
#cv.createTrackbar(outra_barra, janela_controles,0,255,nothing)
'''
Read image and convert it to binary

Extract objects of interest

Extract orientation

Visualize result
'''

'''
leitura da imagem
abre uma janela para escolha da imagem
'''


'''root = tk.Tk()
root.withdraw()
file_path = filedialog.askopenfilename()
print(file_path)'''
src = cv.imread('./png/img0.png',0)

''' 
janela para exibição da imagem com a possibilidade de redimencionamento
'''
cv.namedWindow('src', cv.WINDOW_NORMAL)
cv.namedWindow('bin', cv.WINDOW_NORMAL)

cv.imshow('src',src)

while (1):
    val_thr_bin = cv.getTrackbarPos(barra_threshold_binary, janela_controles)

    # https://docs.opencv.org/master/d7/d4d/tutorial_py_thresholding.html
    ret,threshbin = cv.threshold(src,val_thr_bin,255,cv.THRESH_TOZERO)
    print(ret)
            
    cv.imshow('bin',threshbin)


    '''
    # ao final do programa salva  a imagem se for digitado s
    # ou sai sem salvar se for digitado ESC
    # & 0xFF (adicionado em sistemas 64 bits)
    '''
    k = cv.waitKey(10)  & 0xFF
    if k == 27:         
        cv.destroyAllWindows()
        break
    elif k == ord('s'): 
        cv.imwrite('saida.png',src)
        cv.destroyAllWindows()
        break
