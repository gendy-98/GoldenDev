# -*- coding: utf-8 -*-
"""
Created on Thu Apr 15 14:14:43 2021

@author: ahmed
"""

fr = open('session030721_1.txt', 'r', encoding='utf-16-le')

fw = open('IMPsession030721_1.txt', 'w', encoding='utf-16-le')
data = fr.readline()

while data:
    if (len(data) < 5):
        data = fr.readline()   
        continue
        
    if data.find(" line phy") == -1 & data.find("PLACEHOLDER") == -1 &\
    data.find("PLACEHOLDER") == -1 & data.find("line 820") == -1 &\
	data.find("line 468 ret = 259") == -1 & data.find("line phy 114") == -1 &\
    data.find("line phy 136") == -1 & data.find("PLACEHOLDER") == -1 &\
    data.find("PLACEHOLDER") == -1 & data.find("PLACEHOLDER") == -1 :
        fw.write(data)
        
    data = fr.readline()    
fr.close()
fw.close()        
        