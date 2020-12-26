import json
import jsonlines
import re
import jieba
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
from wordcloud import WordCloud,STOPWORDS
'''
with open(r'C:\Pt_Scrapy\my\items.json','r',encoding='utf-8') as f:
    content = f.read()
#修正格式
content = content.replace('},','}')
with open(r'C:\Pt_Scrapy\my\items_processed.json','w',encoding='utf-8') as f:
    f.write(content)
'''
words = ''

with open(r'C:\Pt_Scrapy\my\items.json','r',encoding='utf-8') as f:
    with open(r'C:\Pt_Scrapy\my\results.txt','w',encoding='utf-8') as g:
        #读取
        for item in jsonlines.Reader(f):
            try:
                #item取出的是列表
                author = '[{}]'.format(item['author'][0])
            except IndexError:
                continue
            #author = re.search('\'(.*)\'',author).group(1)
            comment = item['reply'][0]
            seg_list = jieba.cut(comment)

            comment = ' '.join(seg_list)
            words += comment

            line = author + comment + '\n'
            g.write(line)  

def draw_wordcloud (words,img_array):
    wordcloud = WordCloud(stopwords = STOPWORDS,
                      font_path=r"C:\Pt_Python\classes\text-analysis\HYQiHei-105.ttf",
                      background_color='white',
                      width=2500,
                      height=2000,
                      mask=img_array
                     ).generate(words)
    plt.figure(1,figsize=(13,13))
    plt.imshow(wordcloud)
    plt.axis('off')
    plt.show()

img = Image.open(r'C:\Pt_Python\data\fig_2_5.png')

img_array = np.array(img)

draw_wordcloud(words,img_array)

jieba.cut()
            
    
        
        