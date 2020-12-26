import jieba
with open(r'C:\Pt_Python\data\text\train.txt','r',encoding='utf-8') as f:
    with open(r'C:\Pt_Python\data\text\train_segment_2.txt','a+',encoding='utf-8') as g:
        for line in f:
            line = line[1:].strip()
            seglist = jieba.cut(line)
            sentence = ' '.join(seglist)
            sentence += '\n'
            g.write(sentence)

with open(r'C:\Pt_Python\data\text\test.txt','r',encoding='utf-8') as f:
    with open(r'C:\Pt_Python\data\text\test_segment.txt','a+',encoding='utf-8') as g:
        for line in f:
            seglist = jieba.cut(line)
            sentence = ' '.join(seglist)
            g.write(sentence)