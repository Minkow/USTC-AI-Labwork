#coding:utf-8

import numpy as np
from PIL import Image

def initcenter(data,k):
    centroids = data.copy()
    np.random.shuffle(centroids)
    return centroids[:k]

def kmeans(data,k):
    num,dim = data.shape
    #initial:创建初始点，采用shuffle后取前k个的方法,避免取到同一个点(不存在的.jpg
    centroids = initcenter(data,k)
    cluster = np.array(np.zeros((num,2)))
    #初始点的中心值，下面是维度扩展以实现向量化操作(实验报告里详细说
    points = data[:, np.newaxis, :]

    flag = True
    while flag:
        flag = False
        #距离
        dis = np.sum((points-centroids)**2, axis = 2)
        #分类
        mindis = np.argmin(dis, axis = 1)
        if np.any(cluster != mindis):
            flag = True
            cluster = mindis
            #移动中心
            centroids = [np.average(data[cluster == i], axis=0) for i in range(k)]
    return centroids,cluster

im = Image.open('sea.jpg')
width,height = im.size
size = width*height
#k = input("input k:")
k = 16

data = np.array(im)
dataset = np.reshape(data,(size,3)) #3通道RGB
new = np.zeros((size, 3), dtype = np.uint8)

centroids,cluster = kmeans(dataset,k)

for i in range(size):#将同一聚类的RGB设置为中心点的RGB
    new[i] = centroids[cluster[i]]

new = np.reshape(new, (height, width, 3))
img = Image.fromarray(new)
img.show()
img.save('%d_colors.jpg'%(k))