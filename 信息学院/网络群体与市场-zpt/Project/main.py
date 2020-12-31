import json
from py2neo import Graph
from utils import init_graph
from collections import defaultdict

if __name__ == "__main__":
    print("Initializing...")
    graph = Graph('bolt://localhost:7687',username='neo4j',password='123')
    try:
        init_graph(graph)
    except:
        pass
    figure = json.load(open('data/figure_sorted.json','r',encoding='utf-8'))
    department = json.load(open('data/department_sorted.json','r',encoding='utf-8'))
    print("Initialize finished!")

    while(1):
        print('')
        print("Please input Function #...")
        print("0 for basic information\n1 for query neighbors\n2 for PageRank calculation\n3 for Single Source Shortest Path calculation\n4 for Community Detection\n5 for Betweenness Centrality calculation\n6 for Clustering Coefficient calculation")
        x = input("#")
        if x == 'exit':
            break

        elif x == '0':
            node_counts = graph.run("match (n) return count(n)").evaluate()
            edge_counts = graph.run("match ()-[n:CO_OCCUR_WITH]-() return count(n)").evaluate()
            connected_component = graph.run("CALL gds.alpha.scc.write({nodeProjection: ['Person','Department'],relationshipProjection: 'CO_OCCUR_WITH',writeProperty: 'componentId'}) YIELD setCount, maxSetSize, minSetSize;").data()[0]
            print("There are {} nodes and {} edges, {} strongly connected components, larget size of which is {}".format(node_counts, edge_counts, connected_component['setCount'], connected_component['maxSetSize']))
            
            print("TOP 10 Figures are: {}\nTOP 10 Departments are: {}".format(','.join([k+':'+str(v) for k,v in figure.items()][:10]), ','.join([k+':'+str(v) for k,v in department.items()][:10])))

        elif x == '1':
            name = input("Node:")
            results = graph.run("MATCH ({ name: $name })-[k:CO_OCCUR_WITH]-(m) return distinct m.name,k.weight order by k.weight descending LIMIT 10",name=name).data()
            if(len(results) == 0):
                print("NO MATCHING!")
                continue
            for i in results:
                print('{}, CO_OCCURANCE_COUNTS: {}'.format(i['m.name'],int(i['k.weight']*2)))

        elif x == '2':
            print("calculating PageRank...")
            pagerank = graph.run("CALL gds.pageRank.stream('dense') YIELD nodeId, score RETURN gds.util.asNode(nodeId).name AS name, score ORDER BY score DESC, name ASC LIMIT 20")
            for i,v in enumerate(pagerank):
                print("No.{}: Node:{}, PageRank:{}".format(i+1, v['name'], v['score']))

        elif x=='3':
            source = input("Please input SOURCE NODE: ")
            target = input("Please input TARGET NODE: ")
            a = graph.run("MATCH (n {name: $name}) return n", name=source).evaluate()
            b = graph.run("MATCH (n {name: $name}) return n", name=target).evaluate()

            if not (a and b):
                print("Node Not Valid!")
                continue

            print("calculating Single Source Shortest Path...")
            ssp = graph.run("MATCH (n {name: $name}) CALL gds.alpha.shortestPath.deltaStepping.stream({ nodeQuery:'MATCH (k) WHERE size((k)-[:CO_OCCUR_WITH]->()) > 0 RETURN id(k) AS id', relationshipQuery:'MATCH (n)-[r:CO_OCCUR_WITH]-(m) WHERE size((n)-[:CO_OCCUR_WITH]->()) > 0 RETURN id(n) AS source, id(m) AS target', startNode: n, delta: 3.0 }) YIELD nodeId, distance RETURN gds.util.asNode(nodeId).name AS name, distance AS cost order by cost desc", name=source)
            for v in ssp:
                if v['name'] == target:
                    found = True
                    if v['cost'] == float("inf"):
                        val = v['cost']
                        print("No Path Exists between SOURCE NODE {} and TARGET NODE {}".format(source,target))
                    else:
                        val = int(v['cost'])
                        print("The Shortest Path from SOURCE NODE {} to TARGET NODE {} is {}".format(source,target,val))

        elif x == '4':
            mode = input("for STREAM mode(write all nodes and there community ID into a json file), input 1 \nfor STATISTIC mode (return summary about modularity and communities), input 2\n")
            print("calculating Communities...")
            if mode == '1':
                result = defaultdict(list)
                communities = graph.run("CALL gds.louvain.stream('dense',{relationshipWeightProperty:'weight'}) YIELD nodeId,communityId RETURN nodeId as id, gds.util.asNode(nodeId).name AS name, communityId as communityId order by communityId")
                for i in communities:
                    result[i['communityId']].append(i['name'])
                    
                    # print("Node:{}, Community ID (virtual value):{}".format(i['name'], i['communityId']))
                json.dump(result,open('data/community.json','w',encoding='utf-8'),ensure_ascii=False)
                print("Communities as well as their members are writen into json file in data/community.json!")
            elif mode == '2':
                result = graph.run("CALL gds.louvain.mutate('dense', { mutateProperty: 'communityId' }) YIELD communityCount, modularity").data()[0]
                print("Community Counts:{}, Modularity:{}".format(result['communityCount'], result['modularity']))

        elif x=='5':
            mode = input("for PERSON mode(return PERSON node's betweenness centrality), input 1\nfor DEPARTMENT mode(return DEPARTMENT node's betweenness centrality), input 2\nfor FULL mode(return either node's betweenness centrality), input 3\n")
            if mode == '1':
                print("calculating Betweenness Centrality...")
                berweenness = graph.run("CALL gds.betweenness.stream('person') YIELD nodeId, score RETURN gds.util.asNode(nodeId).name AS name, score ORDER BY score DESC LIMIT 10")
            elif mode == '2':
                print("calculating Betweenness Centrality...")
                berweenness = graph.run("CALL gds.betweenness.stream('department') YIELD nodeId, score RETURN gds.util.asNode(nodeId).name AS name, score ORDER BY score DESC LIMIT 10")
            elif mode == '3':
                print("calculating Betweenness Centrality...")
                berweenness = graph.run("CALL gds.betweenness.stream('sparse') YIELD nodeId, score RETURN gds.util.asNode(nodeId).name AS name, score ORDER BY score DESC LIMIT 10")
            else:
                print("You are KIDDING ME!")
                continue
            for i,v in enumerate(berweenness):
                print("No.{}: Node:{}, Betweenness Centrality:{}".format(i+1, v['name'],v['score']))

        elif x=='6':
            mode = input("for PERSON mode(return PERSON node's clustering coefficient), input 1\nfor DEPARTMENT mode(return DEPARTMENT node's clustering coefficient), input 2\nfor FULL mode(return either node's clustering coefficient), input 3\n")
            if mode == '1':
                print("calculating Clustering Coefficient...")
                coefficient = graph.run("CALL gds.localClusteringCoefficient.stream('person') YIELD nodeId, localClusteringCoefficient RETURN gds.util.asNode(nodeId).name AS name, localClusteringCoefficient ORDER BY localClusteringCoefficient DESC LIMIT 10")
            elif mode == '2':
                print("calculating Clustering Coefficient...")
                coefficient = graph.run("CALL gds.localClusteringCoefficient.stream('department') YIELD nodeId, localClusteringCoefficient RETURN gds.util.asNode(nodeId).name AS name, localClusteringCoefficient ORDER BY localClusteringCoefficient DESC LIMIT 10")
            elif mode == '3':
                print("calculating Clustering Coefficient...")
                coefficient = graph.run("CALL gds.localClusteringCoefficient.stream('dense') YIELD nodeId, localClusteringCoefficient RETURN gds.util.asNode(nodeId).name AS name, localClusteringCoefficient ORDER BY localClusteringCoefficient DESC LIMIT 10")
            else:
                print("You are KIDDING ME!")
                continue
            
            for i,v in enumerate(coefficient):
                print("No.{}: Node:{}, Clustering Coefficient:{}".format(i+1, v['name'], v['localClusteringCoefficient']))
        else:
            print("You are KIDDING ME!")
            continue