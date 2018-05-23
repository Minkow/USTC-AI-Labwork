#coding:utf-8

import random
import word2vec as wv
import numpy as np
from sklearn import metrics
from sklearn.cross_validation import train_test_split
from sklearn.naive_bayes import GaussianNB
from sklearn.linear_model import Ridge
from sklearn.svm import SVC

model = wv.load('a.bin')
f1 = open('data')
f2 = open('vocabulary')
fr1 = f1.readlines()
fr2 = f2.readlines()
f1.close()
f2.close()
good = []
#好评差评集合(label)

for i in range(len(fr1)):
    temp = fr1[i].split(' ',1)
    good.append(int(int(temp[0])>6))
#好评差评按1/0区分

def getFeature(comment):
    com = fr1[comment]
    temp1 = com.split()
    s = np.zeros(200)#特征向量长度为200
    t = 0
    for i in range (len(temp1)-1):
        temp2 = temp1[i+1].split(':')
        word = fr2[int(temp2[0])]
        word = word[:-1]#去末尾换行
        if word in model:
            s += model[word] * int(temp2[1])
            t += float(temp2[1])
    return s/t#加权平均

#n = input("input comment index (start from 0):")
#getFeature(n) 


def nBayesClassfier(traindata, trainlabel, testdata, testlabel, threshold):
    ypred = []
    s = 0
    clf = GaussianNB()
    clf.fit(traindata,trainlabel) #特征值/好评？
    prelist = clf.predict_proba(testdata)
    for i in range (len(prelist)):
        if prelist[i][1] > threshold:#返回值是概率值
            ypred.append(1)
        else:
            ypred.append(0)
    for i in range(len(testlabel)):     
        s += int(ypred[i] == testlabel[i])
    accuracy = 1.0*s/len(testlabel)
    return ypred,accuracy

def lsClassfier(traindata, trainlabel, testdata, testlabel, lamda):#lambda是关键字
    ypred = []
    s = 0
    clf = Ridge (alpha = lamda)
    clf.fit(traindata,trainlabel) #特征值/好评？
    ypred = clf.predict(testdata)#返回值是相关系数
    for i in range(len(ypred)):     
        s += int(int(ypred[i] >= 0.5) == testlabel[i])
    accuracy = 1.0*s/len(ypred)
    return ypred,accuracy

def getKernel(traindata):
    sigma = 0
    m,n = np.shape(traindata)
    for i in xrange(m):
        sigma += np.sum((traindata-traindata[i])**2) #去掉了一个循环，直接算的向量
    sigma /= (m**2)
    return sigma
    

def softsvm(traindata, trainlabel, testdata, testlabel, sigma, C):
    ypred = []
    s = 0
    clf = SVC (C = C, gamma = 1/(sigma**2), max_iter=2500)
    clf.fit(traindata,trainlabel) #特征值/好评？
    ypred = clf.predict(testdata)#返回值是1、0
    for i in range(len(ypred)):     
        s += int(int(ypred[i] == 1) == testlabel[i])   
    accuracy = 1.0*s/len(ypred)
    return ypred,accuracy


#traindata,testdata, trainlabel, testlabel = train_test_split(train,  
 #                                                  target,  
  #                                                 test_size = 0.2,  
   #                                                random_state = 0)  
   
dataset = []

for i in range(len(fr1)): 
    dataset.append(getFeature(i))

#ypred,accuracy = nBayesClassfier(dataset[:40000],good[:40000],dataset[40000:],good[40000:],0.5)
#print(accuracy)

#ypred,accuracy = lsClassfier(dataset[:40000],good[:40000],dataset[40000:],good[40000:],1)
#print(accuracy)

#print(getKernel(dataset[:50000]))


#ypred,accuracy = softsvm(dataset[35000:40000],good[35000:40000],dataset[49000:],good[49000:],sigma,1)
#print(accuracy)

#上面是测试算法时的样例

#下面是统一测试脚本
tarr = [0.5,0.6,0.7,0.75,0.8,0.85,0.9]
larr = [0.0001,0.01,0.1,0.5,1,5,10,100,1000,5000,10000]
sarr = [0.01,0.1,1,10,100]
carr = [1,10,100,1000]

#test:
random.seed(0) #指定随机数种子，重复测试时样本一样
random.shuffle(dataset)
random.seed(0)
random.shuffle(good)

bayes = []
bave = []
for i in range(len(tarr)):
    temp = []
    ypred,accuracy = nBayesClassfier(dataset[:40000],good[:40000],dataset[40000:],good[40000:],tarr[i])
    temp.append(accuracy)
    ypred,accuracy = nBayesClassfier(dataset[:30000]+dataset[40000:],good[:30000]+good[40000:],dataset[30000:40000],good[30000:40000],tarr[i])
    temp.append(accuracy)
    ypred,accuracy = nBayesClassfier(dataset[:20000]+dataset[30000:],good[:20000]+good[30000:],dataset[20000:30000],good[20000:30000],tarr[i])
    temp.append(accuracy)
    ypred,accuracy = nBayesClassfier(dataset[:10000]+dataset[20000:],good[:10000]+good[20000:],dataset[10000:20000],good[10000:20000],tarr[i])
    temp.append(accuracy)
    ypred,accuracy = nBayesClassfier(dataset[10000:],good[10000:],dataset[:10000],good[:10000],tarr[i])
    temp.append(accuracy)
    bayes.append(temp)
    #bave.append(np.mean(temp))
    s = ' t=%f : ave=%f \n'%(tarr[i],np.mean(temp))
    bave.append(s)
print(bayes)
for s in bave:
    print(s)
  

ls = []
lave = []
for i in range(len(larr)):
    temp = []
    ypred,accuracy = lsClassfier(dataset[:40000],good[:40000],dataset[40000:],good[40000:],larr[i])
    temp.append(accuracy)
    ypred,accuracy = lsClassfier(dataset[:30000]+dataset[40000:],good[:30000]+good[40000:],dataset[30000:40000],good[30000:40000],larr[i])
    temp.append(accuracy)
    ypred,accuracy = lsClassfier(dataset[:20000]+dataset[30000:],good[:20000]+good[30000:],dataset[20000:30000],good[20000:30000],larr[i])
    temp.append(accuracy)
    ypred,accuracy = lsClassfier(dataset[:10000]+dataset[20000:],good[:10000]+good[20000:],dataset[10000:20000],good[10000:20000],larr[i])
    temp.append(accuracy)
    ypred,accuracy = lsClassfier(dataset[10000:],good[10000:],dataset[:10000],good[:10000],larr[i])
    temp.append(accuracy)
    ls.append(temp) 
    s = ' l=%f : ave=%f \n'%(larr[i],np.mean(temp))
    lave.append(s)
print(ls)
for s in lave:
    print(s)
#由于求特征值时间较长，调试很麻烦，所以使用测试过的d=50000的数据时的d来进行之后的测试
d = 0.0328


svm = []
save = []
for i in range(len(sarr)):
    for j in range(len(carr)):
        temp = []
        ypred,accuracy = softsvm(dataset[:40000],good[:40000],dataset[40000:],good[40000:],sarr[i]*d,carr[j])
        temp.append(accuracy)
        ypred,accuracy = softsvm(dataset[:30000]+dataset[40000:],good[:30000]+good[40000:],dataset[30000:40000],good[30000:40000],sarr[i]*d,carr[j])
        temp.append(accuracy)
        ypred,accuracy = softsvm(dataset[:20000]+dataset[30000:],good[:20000]+good[30000:],dataset[20000:30000],good[20000:30000],sarr[i]*d,carr[j])
        temp.append(accuracy)
        ypred,accuracy = softsvm(dataset[:10000]+dataset[20000:],good[:10000]+good[20000:],dataset[10000:20000],good[10000:20000],sarr[i]*d,carr[j])
        temp.append(accuracy)
        ypred,accuracy = softsvm(dataset[10000:],good[10000:],dataset[:10000],good[:10000],sarr[i]*d,carr[j])
        temp.append(accuracy)
        svm.append(temp)
        s = ' sigma=%f , C=%f : ave=%f \n'%(sarr[i],carr[j],np.mean(temp))
        save.append(s)
    
print(svm)
for s in save:
    print(s)