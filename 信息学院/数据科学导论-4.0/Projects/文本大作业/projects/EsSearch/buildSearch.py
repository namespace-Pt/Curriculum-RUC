from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk
import re

def gendata(data):
    for message in data:
        yield {
            "_index": "message",
            "doc": message,
        }

es = Elasticsearch()
mapping = {
    'properties':{
        'content':{
            'type':'text',
            'analyzer':'ik_max_word',
            'search_analyzer':'ik_max_word'
        }
    }
}
es.indices.create(index='message')
result=es.indices.put_mapping(index='message',body=mapping)
data=[]
with open(r'C:\Pt_Python\data\text\total_labels.txt','r',encoding='utf-8') as f:
    for line in f:
        message = line[1:].strip()
        label = int(line[0])
        data.append({
            "content":message,
            "label":label
        })

actions = [
    {
        "_index": "message",
        "_source": message,
    }
    for message in data
]
a,b=bulk(es,actions,index='message')
print(a,b)
