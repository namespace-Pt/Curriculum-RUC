from elasticsearch import Elasticsearch
es = Elasticsearch()
print('type in your keys:         ')
keys = input()
model = {
    'query':{
        'match':{
            'content':keys
        }
    }
}
result = es.search(body=model)
messages = result['hits']['hits']
print('there are {} results found in total , how many results you would like to attain:'.format(result['hits']['total']['value']))
total = input()
for i in range(int(total)):
    isSpam = messages[i]['_source']['label']
    content = messages[i]['_source']['content']
    if isSpam == 1:
        print('****垃圾短信****'+content)
    else:
        print('****正常短信****'+content)
    