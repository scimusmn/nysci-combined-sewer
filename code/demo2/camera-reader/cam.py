FRAME_COUNT = 8
FRAME_LEN = 1000
import cv2 as cv
import numpy as np

camera = cv.VideoCapture(0, cv.CAP_DSHOW)

frames = []

for count in range(0, FRAME_COUNT):
  print(count)
  ret, frame = camera.read()
  frames.append(frame)
  cv.imshow("window", frame)
  cv.waitKey(FRAME_LEN)


def threshold(frame, lowLevel, highLevel):
  hsv = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
  v = hsv[::1, ::1, 2]
  ret, low = cv.threshold(v, lowLevel, 1, cv.THRESH_BINARY)
  print(low)
  ret, high = cv.threshold(v, highLevel, 255, cv.THRESH_BINARY)
  gray = np.multiply(v, low)
  gray = np.maximum(gray, high) 
  return gray



def binary(frames, low, high):
  h, w, _ = frames[0].shape
  shape = (h, w)
  dtype = np.uint32
  binary = np.zeros(shape, dtype)
  ignore = np.zeros(shape, np.dtype(bool))
  shift = 0
  for f in frames:
    brightness = cv.cvtColor(f, cv.COLOR_BGR2HSV)[::1, ::1, 2]
    oneMask = np.zeros(shape, np.dtype(bool))
    oneMask[brightness > high] = True
    nonZeroMask = np.zeros(shape, np.dtype(bool))
    nonZeroMask[brightness > low] = True
    mask = np.zeros(shape, dtype)
    mask[oneMask] = 1
    mask = np.left_shift(mask, shift)
    binary = np.bitwise_or(mask, binary)
    shift += 1
    ignore[np.logical_and(np.logical_not(oneMask), nonZeroMask)] = True
  return (binary, ignore)


 
 

levels = (30, 200)
toSample = False
samplePoint = (0, 0)
def drawPreview(wait=-1):
  global toSample, samplePoint
  low, high = levels
  b, ignore = binary(frames, low, high)
  if toSample:
    x, y = samplePoint
    print(b[y, x])
    toSample = False
  preview = frames[0].copy()
  preview[ignore] = [ 0, 0, 255 ];
  cv.imshow("preview", preview)
  return cv.waitKey(1)
 
def setLow(x):
  global levels
  _, hi = levels
  levels = (x, hi)

def setHigh(x):
  global levels
  lo, _ = levels
  levels = (lo, x)

def onMouse(event, x, y, flags, _):
  global toSample, samplePoint
  if event == cv.EVENT_LBUTTONDOWN:
    print(x, y)
    samplePoint = (x, y)
    toSample = True

cv.namedWindow("preview")
cv.setMouseCallback("preview", onMouse)
cv.createTrackbar("low", "preview", 0, 255, setLow)
cv.createTrackbar("high", "preview", 0, 255, setHigh)

while drawPreview() == -1:
  None

camera.release()
