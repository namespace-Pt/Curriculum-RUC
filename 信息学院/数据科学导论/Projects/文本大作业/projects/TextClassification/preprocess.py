from collections import defaultdict
from gensim.utils import simple_preprocess
import re

with open(r'C:\Pt_Python\data\text\train_segment_2.txt','r',encoding='utf-8') as f:
    documents = []
    for line in f:

        documents.append(simple_preprocess(line,deacc=True,min_len=1))

stoplist = ['啊','阿','呀','嘿','哦','奥','噢','咦','哎']
texts = [[word for word in document if word not in stoplist] for document in documents]
frequency = defaultdict(int)
for text in texts:
    for word in text:
        frequency[word] += 1

texts = [
    [word for word in line if frequency[word] >= 5]
    for line in texts
]

with open(r'C:\Pt_Python\data\text\train_preprocess.txt','w',encoding='utf-8') as f:
    for text in texts:
        sentence = ' '.join(text)
        sentence = re.sub('x+','x',sentence)
        sentence = sentence+'\n'
        f.write(sentence)

with open(r'C:\Pt_Python\data\text\test_segment.txt','r',encoding='utf-8') as f:
    documents = []
    for line in f:

        documents.append(simple_preprocess(line,deacc=True))

stoplist = ['啊','阿','呀','嘿','哦','奥','噢','咦','哎']
texts = [[word for word in document if word not in stoplist] for document in documents]
frequency = defaultdict(int)
for text in texts:
    for word in text:
        frequency[word] += 1

texts = [
    [word for word in line if frequency[word] >= 3]
    for line in texts
]

with open(r'C:\Pt_Python\data\text\test_preprocess.txt','w',encoding='utf-8') as f:
    for text in texts:
        sentence = ' '.join(text)
        sentence = re.sub('x+','x',sentence)
        sentence = sentence+'\n'
        f.write(sentence)