#!/home/pi/.virtualenvs/cv/bin/python2.7
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import sys
import os
import cv2
import numpy as np


def getImageFromCamera():
	camera = PiCamera()
	rawCapture = PiRGBArray(camera)
	time.sleep(0.1)
	camera.capture(rawCapture,format="bgr")
	image = rawCapture.array
	return image


def sendImage(socket,im):
	
	
	encode_param=[int(cv2.IMWRITE_JPEG_QUALITY),90]
	result, imgencode = cv2.imencode(".jpg",im,encode_param)
	data = np.array(imgencode)
	stringData = data.tostring()

	x,y,z = im.shape

	form = str(x)+","+str(y)+","+str(len(stringData)).ljust(16)+"\0"

	ret = os.write(socket,form.encode())

	#wait for cmd
	ret = os.read(socket,1024)
	
	ret = os.write(socket,stringData)
	
	

def main():
	

	socket = int(sys.argv[1])
		
	image = getImageFromCamera()
	sendImage(socket,image)

if __name__ == '__main__':
	main()