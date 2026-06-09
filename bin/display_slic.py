from imageio.v3 import imread
from skimage.color import label2rgb
import matplotlib.pyplot as plt

import sys

img = imread(sys.argv[1])
lbl = label2rgb(img)
plt.imshow(lbl)
plt.show()