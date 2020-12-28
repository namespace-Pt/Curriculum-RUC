import thulac
import pandas
import json
import numpy as np
from tqdm import tqdm
from py2neo import Node,Relationship
from py2neo.matching import NodeMatcher
from collections import defaultdict

pts_refer = {'np':'Person','ni':'Department'}

def preprocess(fin,fout):
    """
        preprocess data, drop nan then reset indexes
    """
    data = pandas.read_table(fin) 
    index = data[data.isnull().any(axis=1)].index
    data.drop(index,inplace=True)
    data.reset_index(inplace=True,drop=True)
    data['meta'] = data['meta'].map(lambda x: x[4:])

    data.to_csv(fout,index=None,sep='\t')

def tokenize(fin,fout):
    """
        tokenize strings of fin, output into fout
    """
    data = pandas.read_table(fin)
    model = thulac.thulac(filt=True)
    stopList = [i for i in r'【{}%^>年月日在了等为要有大小… ']
    for idx,row in tqdm(data.iterrows()):
        # print(type(row['title']))
        row['title'] = ' '.join(['{}:{}'.format(i[0],i[1]) for i in model.cut(row['title']) if i[0] not in stopList])
        row['text'] = ' '.join(['{}:{}'.format(i[0],i[1]) for i in model.cut(row['text'])  if i[0] not in stopList])
    data.to_csv(fout,index=None,sep='\t')

def build_vocab(fin):
    """
        build vocabulary and part-of-speech reference
    """
    word_count = defaultdict(int)
    vocab = {}
    part_of_speech = {}

    with open(fin, 'r', encoding='utf-8') as f:
        for idx,line in enumerate(f):
            if idx == 0:
                continue
            try:
                url,source,date,title,text = line.strip('\n').split('\t')
                title_words = [i.split(':') for i in title.split(' ') if i != '']
                text_words = [i.split(':') for i in text.split(' ') if i != '']
            except:
                print(len(line.split('\t')),idx)
                break
            words = title_words + text_words
            for word in words:
                try:
                    part_of_speech[word[0]] = word[1]
                    word_count[word[0]] += 1
                    if word[0] not in vocab:
                        vocab[word[0]] = len(vocab) + 1
                except:
                    pass
    vocab['<PAD>'] = 0

    f = open('data/word_count.json','w',encoding='utf-8')
    json.dump(word_count,f,ensure_ascii=False)
    f.close()

    f = open('data/part_of_speech.json','w',encoding='utf-8')
    json.dump(part_of_speech,f,ensure_ascii=False)
    f.close()

    f = open('data/vocab.json','w',encoding='utf-8')
    json.dump(vocab,f,ensure_ascii=False)
    f.close()

    return word_count, part_of_speech, vocab

def get_figure(word_count=None,part_of_speech=None):
    """
        get hot figure
    """
    if not word_count:
        word_count = json.load(open('data/word_count.json','r',encoding='utf-8'))
    if not part_of_speech:
        part_of_speech = json.load(open('data/part_of_speech.json','r',encoding='utf-8'))

    figure = {k:v for k,v in word_count.items() if part_of_speech[k] == 'np' and k[0] != '"' and len(k) > 1}

    del figure['习近平在宁德']

    figure_sorted = {k: v for k, v in sorted(figure.items(), key=lambda item: item[1], reverse=True)}

    fig2idx = {}
    for k,v in figure_sorted.items():
        fig2idx[k] = len(fig2idx)
    
    id2fig = {v:k for k,v in fig2idx.items()}

    f = open('data/figure_sorted.json','w',encoding='utf-8')
    json.dump(figure_sorted,f,ensure_ascii=False)
    f.close()

    f = open('data/fig2idx.json','w',encoding='utf-8')
    json.dump(fig2idx,f,ensure_ascii=False)
    f.close()
    
    f = open('data/id2fig.json','w',encoding='utf-8')
    json.dump(id2fig,f,ensure_ascii=False)
    f.close()
    
    return figure_sorted,fig2idx,id2fig

def get_department(word_count=None, part_of_speech=None):
    """
        get hot department
    """
    if not word_count:
        word_count = json.load(open('data/word_count.json','r',encoding='utf-8'))
    if not part_of_speech:
        part_of_speech = json.load(open('data/part_of_speech.json','r',encoding='utf-8'))

    department = {k:v for k,v in word_count.items() if part_of_speech[k] == 'ni' and k[0] != '"' and len(k) > 1}

    department_sorted = {k: v for k, v in sorted(department.items(), key=lambda item: item[1], reverse=True)}

    dept2idx = {}
    for k,v in department_sorted.items():
        dept2idx[k] = len(dept2idx)
    
    id2dept = {v:k for k,v in dept2idx.items()}

    f = open('data/department_sorted.json','w',encoding='utf-8')
    json.dump(department_sorted,f,ensure_ascii=False)
    f.close()

    f = open('data/dept2idx.json','w',encoding='utf-8')
    json.dump(dept2idx,f,ensure_ascii=False)
    f.close()
    
    f = open('data/id2dept.json','w',encoding='utf-8')
    json.dump(id2dept,f,ensure_ascii=False)
    f.close()
    
    return department_sorted,dept2idx,id2dept

def get_entity(word_count=None,part_of_speech=None):
    """
        get hot entity
    """
    if not word_count:
        word_count = json.load(open('data/word_count.json','r',encoding='utf-8'))
    if not part_of_speech:
        part_of_speech = json.load(open('data/part_of_speech.json','r',encoding='utf-8'))

    entity = {k:{'count':v, 'type':part_of_speech[k]} for k,v in word_count.items() if (part_of_speech[k] == 'np' or part_of_speech[k] == 'ni') and k[0] != '"' and len(k) > 1}

    del entity['习近平在宁德']

    entity_sorted = {k: v for k, v in sorted(entity.items(), key=lambda item: item[1]['count'], reverse=True)}

    entity2idx = {}
    for k,v in entity_sorted.items():
        entity2idx[k] = {'id':len(entity2idx), 'type':part_of_speech[k]}
    
    id2entity = {v['id']:{'var':k, 'type':part_of_speech[k]} for k,v in entity2idx.items()}

    f = open('data/entity_sorted.json','w',encoding='utf-8')
    json.dump(entity_sorted,f,ensure_ascii=False)
    f.close()

    f = open('data/entity2idx.json','w',encoding='utf-8')
    json.dump(entity2idx,f,ensure_ascii=False)
    f.close()
    
    f = open('data/id2entity.json','w',encoding='utf-8')
    json.dump(id2entity,f,ensure_ascii=False)
    f.close()
    
    return entity_sorted,entity2idx,id2entity

def build_adj_fig(fin='data/tokenized.txt'):
    """
        build adjacency matrix of undirected co-occurrance graph
    """
    with open('data/fig2idx.json','r',encoding='utf-8') as f:
        fig2idx = json.load(f)

    adjacency = np.zeros((len(fig2idx),len(fig2idx)),dtype=np.int)

    with open(fin, 'r', encoding='utf-8') as f:
        for idx,line in enumerate(f):
            url,source,date,title,text = line.strip('\n').split('\t')
            title_figures = [i.split(':')[0] for i in title.split(' ') if i.split(':')[0] in fig2idx]
            text_figures = [i.split(':')[0] for i in text.split(' ') if i.split(':')[0] in fig2idx]
            
            words = list(set(title_figures + text_figures))
            # print(words)
            for i,figure1 in enumerate(words):
                # print(figure1)
                for j,figure2 in enumerate(words[i+1:]):
                    # print(figure2)
                    adjacency[fig2idx[figure1],fig2idx[figure2]] +=  1

    adj = np.zeros((len(fig2idx),len(fig2idx)),dtype=np.int)        
    for i in range(adjacency.shape[0]):
        for j in range(i,adjacency.shape[1]):
            adj[i][j] = adjacency[i][j] + adjacency[j][i]
    
    np.save('data/adjacency_fig.npy',adj)
    return adj

def build_adj_dept(fin='data/tokenized.txt'):
    """
        build adjacency matrix of undirected co-occurrance graph
    """
    with open('data/dept2idx.json','r',encoding='utf-8') as f:
        dept2idx = json.load(f)

    adjacency = np.zeros((len(dept2idx),len(dept2idx)),dtype=np.int)

    with open(fin, 'r', encoding='utf-8') as f:
        for idx,line in enumerate(f):
            url,source,date,title,text = line.strip('\n').split('\t')
            title_depts = [i.split(':')[0] for i in title.split(' ') if i.split(':')[0] in dept2idx]
            text_depts = [i.split(':')[0] for i in text.split(' ') if i.split(':')[0] in dept2idx]
            
            words = list(set(title_depts + text_depts))
            # print(words)
            for i,dept1 in enumerate(words):
                # print(dept1)
                for j,dept2 in enumerate(words[i+1:]):
                    # print(dept2)
                    adjacency[dept2idx[dept1],dept2idx[dept2]] +=  1

    adj = np.zeros((len(dept2idx),len(dept2idx)),dtype=np.int)        
    for i in range(adjacency.shape[0]):
        for j in range(i,adjacency.shape[1]):
            adj[i][j] = adjacency[i][j] + adjacency[j][i]
    
    np.save('data/adjacency_dept.npy',adj)
    return adj

def build_adj_entity(fin='data/tokenized.txt'):
    """
        build adjacency matrix of undirected co-occurrance graph
    """
    with open('data/entity2idx.json','r',encoding='utf-8') as f:
        entity2idx = json.load(f)

    adjacency = np.zeros((len(entity2idx),len(entity2idx)),dtype=np.int)

    with open(fin, 'r', encoding='utf-8') as f:
        for idx,line in enumerate(f):
            url,source,date,title,text = line.strip('\n').split('\t')
            title_entitys = [i.split(':')[0] for i in title.split(' ') if i.split(':')[0] in entity2idx]
            text_entitys = [i.split(':')[0] for i in text.split(' ') if i.split(':')[0] in entity2idx]
            
            words = list(set(title_entitys + text_entitys))
            # print(words)
            for i,entity1 in enumerate(words):
                # print(entity1)
                for j,entity2 in enumerate(words[i+1:]):
                    # print(entity2)
                    adjacency[entity2idx[entity1],entity2idx[entity2]] +=  1

    adj = np.zeros((len(entity2idx),len(entity2idx)),dtype=np.int)        
    for i in range(adjacency.shape[0]):
        for j in range(i,adjacency.shape[1]):
            adj[i][j] = adjacency[i][j] + adjacency[j][i]
    
    np.save('data/adjacency_entity.npy',adj)
    return adj

def build_import_csv():
    """
        build basic csv file for LOAD CSV in neo4j
    """
    with open('data/id2fig.json','r',encoding='utf-8') as f:
        id2fig = json.load(f)
    with open('data/fig2idx.json','r',encoding='utf-8') as f:
        fig2idx = json.load(f)
    adj = np.load('data/adjacency_fig.npy')

    with open('data/Nodes.csv', 'w', newline='', encoding='utf-8') as csvfile:
        for k,v in fig2idx.items():
            csvfile.write(k + '\n')

    with open('data/Relationships.csv', 'w', newline='', encoding='utf-8') as csvfile:
        # writer = csv.writer(csvfile)
        csvfile.write(','.join(['node1','node2','weight']) + '\n')
        for i in range(adj.shape[0]):
            for j in range(adj.shape[1]):
                if adj[i][j] > 1:
                    csvfile.write(','.join([id2fig[str(i)],id2fig[str(j)],str(adj[i][j])]) + '\n')

def build_import_csv_whole():
    """
        build basic csv file for LOAD CSV in neo4j
    """
    # with open('data/id2entity.json','r',encoding='utf-8') as f:
    #     id2entity = json.load(f)
    # with open('data/fig2idx.json','r',encoding='utf-8') as f:
    #     fig2idx = json.load(f)
    # with open('data/dept2idx.json','r',encoding='utf-8') as f:
    #     dept2idx = json.load(f)
    with open('data/entity2idx.json','r',encoding='utf-8') as f:
        entity2idx = json.load(f)

    adj = np.load('data/adjacency_entity.npy')

    # with open('data/Nodes_fig.csv', 'w', newline='', encoding='utf-8') as csvfile:
    #     for k,v in fig2idx.items():
    #         csvfile.write(k + '\n')
    
    # with open('data/Nodes_dept.csv', 'w', newline='', encoding='utf-8') as csvfile:
    #     for k,v in dept2idx.items():
    #         csvfile.write(k + '\n')

    with open('data/Nodes_whole.csv', 'w', newline='', encoding='utf-8') as csvfile:
        csvfile.write(','.join(['entityID:ID','name',':LABEL']) + '\n')

        for k,v in entity2idx.items():
            csvfile.write(','.join([str(v['id']),k,pts_refer[v['type']]]) + '\n')

    with open('data/Relationships_whole.csv', 'w', newline='', encoding='utf-8') as csvfile:
        # writer = csv.writer(csvfile)
        csvfile.write(','.join([':START_ID','weight',':END_ID',':TYPE']) + '\n')
        for i in range(adj.shape[0]):
            for j in range(adj.shape[1]):
                if adj[i][j] > 1:
                    csvfile.write(','.join([str(i),str(adj[i][j]/2),str(j),'CO_OCCUR_WITH']) + '\n')
                    csvfile.write(','.join([str(j),str(adj[i][j]/2),str(i),'CO_OCCUR_WITH']) + '\n')
                    
def build_graph_csv(graph, load_whole=True):
    """ deprecated
        import nodes and relationships into Neo4j
    """
    if load_whole:
        graph.run("LOAD CSV FROM 'file:///Nodes_fig.csv' AS line CREATE (:Person {name: line[0]})")
        graph.run("LOAD CSV FROM 'file:///Nodes_dept.csv' AS line CREATE (:Department {name: line[0]})")
        try:
            graph.run("CREATE INDEX FOR (a:Person) ON (a.name)")
        except:
            pass
        try:
            graph.run("CREATE INDEX FOR (a:Department) ON (a.name)")
        except:
            pass

        graph.run("LOAD CSV WITH HEADERS FROM 'file:///Relationships_whole.csv' AS row MATCH (s {name: row['node1']}) MATCH (t {name: row['node2']}) CREATE (s)-[:CO_OCCUR_WITH {weight:toFloat(row['weight'])/2 }]->(t) CREATE (t)-[:CO_OCCUR_WITH {weight:toFloat(row['weight'])/2 }]->(s)")

    else:
        graph.run("LOAD CSV FROM 'file:///Nodes_fig.csv' AS line CREATE (:Person {name: line[0]})")
        try:
            graph.run("CREATE INDEX FOR (a:Person) ON (a.name)")
        except:
            pass
        graph.run("LOAD CSV WITH HEADERS FROM 'file:///Relationships.csv' AS row MATCH (s {name: row['node1']}) MATCH (t {name: row['node2']}) CREATE (s)-[:CO_OCCUR_WITH {weight:toFloat(row['weight'])/2 }]->(t) CREATE (t)-[:CO_OCCUR_WITH {weight:toFloat(row['weight'])/2 }]->(s)")

def build_graph(graph, adj=None):
    """ deprecated
        build neo4j graph according to adj(adjacency matrx), and initialize PageRank catelog
    """
    if not adj:
        adj = np.load('data/adjacency_fig.npy')
    
    with open('data/fig2idx.json','r',encoding='utf-8') as f:
        fig2idx = json.load(f)
    with open('data/id2fig.json','r',encoding='utf-8') as f:
        id2fig = json.load(f)

    nodes = NodeMatcher(graph)
    for i in range(adj.shape[0]):
        node = Node('Person',name=id2fig[str(i)])
        graph.create(node)

    for i in range(adj.shape[0]):
        print(i)

        for j in range(i,adj.shape[1]):
            if adj[i,j] > 0:
                s = nodes.match('Person',name=id2fig[str(i)]).first()
                t = nodes.match('Person',name=id2fig[str(j)]).first()

                r_1 = Relationship(s,'CO_OCCUR_WITH',t,weight=adj[i,j]/2)
                r_2 = Relationship(t,'CO_OCCUR_WITH',s,weight=adj[i,j]/2)
                graph.create(r_1)
                graph.create(r_2)

def init_graph(graph):
    """
        init graph catalog
    """
    graph.run("CALL gds.graph.create('person','Person',{CO_OCCUR_WITH:{orientation:'undirected'}},{relationshipProperties: 'weight'})")
    graph.run("CALL gds.graph.create('department','Department',{CO_OCCUR_WITH:{orientation:'undirected'}},{relationshipProperties: 'weight'})")
    graph.run("CALL gds.graph.create('sparse',['Person','Department'],{CO_OCCUR_WITH:{orientation:'undirected'}},{relationshipProperties: 'weight'})")
    graph.run("CALL gds.graph.create.cypher('dense','MATCH (k) WHERE size((k)-[:CO_OCCUR_WITH]->()) > 0 RETURN id(k) AS id','MATCH (n)-[r:CO_OCCUR_WITH]-(m) WHERE size((n)-[:CO_OCCUR_WITH]->()) > 0 RETURN id(n) AS source, id(m) AS target, r.weight AS weight')")