from elasticsearch import Elasticsearch
es = Elasticsearch()
#result = es.indices.create(index='zpt')
result = es.indices.delete(index='zpt')
print(result)