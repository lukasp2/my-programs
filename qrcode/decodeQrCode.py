#!/usr/bin/env python
import cv2
'''
import pyzbar.pyzbar as pyzbar

decodedObjects = pyzbar.decode(cv2.imread("QR_1.png"))

print(decodedObjects)
'''

img = cv2.imread("QR_1.png")

data, bbox, straight_qrcode = cv2.QRCodeDetector().detectAndDecode(img)

print(data, bbox, straight_qrcode)

