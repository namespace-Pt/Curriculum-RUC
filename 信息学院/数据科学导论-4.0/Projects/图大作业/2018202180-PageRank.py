import re
import numpy
import time
import pandas
from collections import defaultdict
from scipy import sparse

'''
class Matrix:

    def __init__(self,n):
        self.out_degree_vec = numpy.zeros((1,n),dtype=int)
        self.matrix = defaultdict(list)
        self.n = n

    def merge(self,edge_list):
        for each in edge_list:
            self.matrix[each[1]].append(each)
            
            self.out_degree_vec[myTuple[0]] += 1

    def process(self):
        index = numpy.arange(0,self.n)
        cursor = index[self.out_degree_vec == 0]

        #出度为0的顶的序号
        zeros = index[cursor[0]]
        
        #出度全部加一
        for row in zeros:
            for column in range(self.n): 
                self.matrix[column].append(row)

        self.out_degree_vec[cursor] = self.n
    
    def _multiply(self):
        self.out_degree_vec = 1/self.out_degree_vec

        for row in range(self.n):
            for column in range(self.n): 
                self.matrix[column] = [i*self.out_degree_vec[row] for i in self.matrix[column]]
        
        return self.matrix
'''

#读文件 
def readFile(input_file_name):
    edge_list = []
    #以顶点ID为索引，查找其序号
    node_to_index = {}
    #以顶点序号为索引，查找其ID
    index_to_node = {}
    index = 0
    with open(input_file_name,'r') as f:
        for line in f:
            #利用,分隔
            group = line.strip().split(',')
            #group = re.split(r'\s',line.strip())

            a = group[0]
            b = group[1]

            #利用字典快速查找索引已存在的顶
            if a in node_to_index:
                a_index = node_to_index[a]
            #如不存在则更新
            else:
                node_to_index[a] = index
                a_index = index
                index_to_node[index] = a
                index += 1

            if b in node_to_index:
                b_index = node_to_index[b]
            else:
                node_to_index[b] = index
                b_index = index
                index_to_node[index] = b
                index += 1

            edge_list.append((a_index,b_index))
    
    return edge_list,index_to_node

def PageRank(input_file_name,damping_factor):

    edge_list,index_to_node = readFile(input_file_name)
    n = len(index_to_node)
    
    #初始化邻接矩阵
    cm = sparse.lil_matrix((n,n))
    for couple in edge_list:
        cm[couple[0],couple[1]] = 1

    #计算出度
    out = cm.sum(axis=1).A1
    
    #记录出度为0的位置
    cursor = (out == 0)

    #取出度大于零的所有位置，直接转倒数，省略对角矩阵求逆
    out[out>0] = 1/out[out>0]
    out_matrix_inv = sparse.diags(out)

    #更新矩阵
    cm = out_matrix_inv @ cm

    #节点分数向量，用传播实现初始化
    p = numpy.ones((1,n))
    p *= 1/n

    #处理悬挂点
    p_fix_dangling = numpy.zeros((1,n)).T
    p_fix_dangling[cursor] = 1/n

    #初始化前序节点分数向量
    p_pre = numpy.zeros((1,n))
    
    
    while((p - p_pre)@(p - p_pre).T > 1e-6):
        #将全1矩阵转化为求和，同样利用传播
        sum_p = p.sum()
        p_damping = numpy.ones((1,n))
        p_damping *= sum_p
        #更新前一个节点分数向量
        p_pre = p

        #计算p
        p = damping_factor * p @ cm + (1 - damping_factor)/n * p_damping
        #将悬挂点的权重加上
        p += p @ p_fix_dangling
    
    #返回顶点列表
    node_list_in_descending_order = [index_to_node[i] for i in numpy.argsort(-p[0])]
    
    return node_list_in_descending_order#p,index_to_node

def PPR(input_file_name,damping_factor):
    edge_list,index_to_node = readFile(input_file_name)
    n = len(index_to_node)

    #初始化邻接矩阵
    cm = sparse.lil_matrix((n,n))
    for couple in edge_list:
        cm[couple[0],couple[1]] = 1

    #计算出度
    out = cm.sum(axis=1).A1
    
    #记录出度为0的位置
    cursor = (out == 0)

    #取出度大于零的所有位置，直接转倒数，省略对角矩阵求逆
    out[out>0] = 1/out[out>0]
    out_matrix_inv = sparse.diags(out)

    #更新矩阵
    cm = out_matrix_inv @ cm

    #初始化节点分数向量
    p = numpy.zeros((1,n))
    p[0,0:50] = 1/50

    #处理悬挂点
    p_fix_dangling = numpy.zeros((1,n)).T
    p_fix_dangling[cursor] = 1/n
    
    #初始化前序向量和用户偏好
    p_pre = numpy.zeros((1,n))
    p_0 = p

    while((p - p_pre)@(p - p_pre).T > 1e-6):
        #更新前序向量
        p_pre = p
        #计算p
        p = damping_factor * p @ cm + (1-damping_factor) * p_0
        #加入悬挂点权重
        p += p @ p_fix_dangling

    node_list_in_descending_order = [index_to_node[i] for i in numpy.argsort(-p[0])]

    return node_list_in_descending_order#p,index_to_node


#start = time.time()
#result = PageRank(r'C:\Pt_Python\data\graph\g2.txt',0.85)
#print(result)
#result,index_to_node = PageRank(r'C:\Pt_Python\data\graph\soc-Epinions1.txt',0.85)
#end = time.time()
#node_list_in_descending_order = [index_to_node[i] for i in numpy.argsort(-result[0])][0:10]
#indexes = [i for i in numpy.argsort(-result[0])][0:10]
#print("time:{}\ntop10:{}\nscore:{}".format((end - start),node_list_in_descending_order,result[0,indexes]))
#print(result[0:10])