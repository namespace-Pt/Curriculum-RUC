from nltk.classify import NaiveBayesClassifier
from collections import defaultdict
from gensim import corpora
import re
def getDict():
    vocabulary = {}
    for sentence in sentences:
        for word in sentence:
            vocabulary[word] = True
    print(vocabulary)
    for index,sentence in enumerate(sentences):
        corpus_dict = dict((w,0) for w in vocabulary.keys())
        for word in sentence:
            corpus_dict[word] += 1
        corpus_total.append((corpus_dict,labels[index]))

sentences = []
labels = []
with open(r'C:\Pt_Python\data\text\train_preprocess.txt','r',encoding='utf-8') as f:
    for line in f:
        line = line.strip()
        sentences.append(re.split(' ',line))



with open(r'C:\Pt_Python\data\text\train.txt','r',encoding='utf-8') as f:
    for line in f:
        labels.append(int(line[0]))

corpus_total = []
'''
getDict()
'''
