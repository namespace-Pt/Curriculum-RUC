import pandas
import re
import json
import numpy

'''
class Student():
    # manage students, only initialize with curriculums and grades
    def __init__(self,id,name,gender,major,curriculum,grade):
        self._id = id
        self._name = name
        self._gender = gender
        self._major = major
        self._curriculum = [curriculum]

class Curriculum():
    def __init__(self,id,name,credit,student_id,grade):
        self._id = id
        self._name = name
        self._credit = credit
        self._students = [student_id]

'''



class int64Encoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, numpy.int64):
            return int(obj)
        else:
            return super().default(obj)

def load_data():
    with open('lab1_students.json','r',encoding='utf-8') as f:
        students = json.load(f)
    with open('lab1_curriculums.json','r',encoding='utf-8') as f:
        curriculums = json.load(f)

    with open('lab1_stuNM2ID.json','r',encoding='utf-8') as f:
        stuNM2ID = json.load(f)
    
    with open('lab1_curNM2ID.json','r',encoding='utf-8') as f:
        curNM2ID = json.load(f)
    
    return students,curriculums,stuNM2ID,curNM2ID

def add_data(students,curriculums,stuNM2ID,curNM2ID):
    data = pandas.read_csv(r'D:\Data\Class_data\DB_data\lab1.csv')
    data['stuID'] = data['stuID'].astype(str)
    data['curID'] = data['curID'].astype(str)

    for i in range(len(data)):
        record = data.loc[i]
        stuID = record['stuID']
        stuNM = record['stuNM']
        gender = record['gender']
        age = record['age']
        major = record['major']
        curNM = record['curNM']
        curID = record['curID']
        credit = record['credit']
        grade = record['grade']

        if stuID not in students:
            student = {
                'stuID':stuID,
                'stuNM':stuNM,
                'gender':gender,
                'age':age,
                'major':major,
                'credit':credit,
                'average':grade,
                'grades':{
                    curID:grade,
                }
            }
            students[stuID] = student
            if stuNM not in stuNM2ID:
                stuNM2ID[stuNM] = [stuID]
            else:
                stuNM2ID[stuNM].append(stuID)
                
        else:
            student = students[stuID]

            if curID not in student['grades']:
                student['grades'][curID] = grade

                _credit = student['credit']
                _average = student['average']
                student['credit'] = _credit + credit

                student['average'] = (_credit * _average + credit * grade)/student['credit']


        if curID not in curriculums:
            curriculum = {
                'curID':curID,
                'curNM':curNM,
                'credit':credit,
                'average':grade,
                'grades':{
                    stuID:grade
                }
            }
            curriculums[curID] = curriculum
            if curNM not in curNM2ID:
                curNM2ID[curNM] = [curID]
            else:
                curNM2ID[curNM].append(curID)
        else:
            curriculum = curriculums[curID]
            if stuID not in curriculum['grades']:
                curriculums[curID]['grades'][stuID] = grade
                curriculums[curID]['average'] = sum(curriculums[curID]['grades'].values())/len(curriculums[curID]['grades'])
            
    with open('lab1_students.json','w',encoding='utf-8') as f:
        f.write(json.dumps(students,ensure_ascii=False,cls=int64Encoder))

    with open('lab1_curriculums.json','w',encoding='utf-8') as f:
        f.write(json.dumps(curriculums,ensure_ascii=False,cls=int64Encoder))

    with open('lab1_stuNM2ID.json','w',encoding='utf-8') as f:
        f.write(json.dumps(stuNM2ID,ensure_ascii=False,cls=int64Encoder))
    
    with open('lab1_curNM2ID.json','w',encoding='utf-8') as f:
        f.write(json.dumps(curNM2ID,ensure_ascii=False,cls=int64Encoder))

    return students,curriculums,stuNM2ID,curNM2ID
        
def query(expression,students,curriculums,stuNM2ID,curNM2ID):
    # select student where stuID=202180
    # select curriculum where curID=5810
    # select student where stuNM=张配天
    # select curriculum where curNM=数据库概论
    
    # delete record where stuID=202180,curID=5810
    # revise grade=99 where stuID=202180,curID=5810
    
    # select average where stuID=202180
    # select average where curID=5810
    # select cur_num where stuID=202180

    exp_sep = re.split(' ',expression)
    
    print('*'*10)

    if exp_sep[0] == 'select':

        source = exp_sep[3].split('=')
        key = source[0]
        value = source[1]
        if key == 'stuID':
            target = students[value]
            if exp_sep[1] == 'average':
                print('stuID:%s, stuNM:%s, average_grade:%s' 
                % (target['stuID'],target['stuNM'],target['average']))
            
            elif exp_sep[1] == 'student':
                print('stuID:%s, stuNM:%s, gender:%s, age:%s, major:%s, curriculums:%s' 
                % (target['stuID'],target['stuNM'],target['gender'],target['age'],target['major'],','.join(target['grades'].keys())))

            elif exp_sep[1] == 'cur_num':
                print('stuID:%s, stuNM:%s, cur_num:%s' 
                % (target['stuID'],target['stuNM'],len(target['grades'])))
            
            else:
                print('Invalid Syntax!')

        elif key == 'stuNM':
            for each in stuNM2ID[value]:
                target = students[each]
                if exp_sep[1] == 'average':
                    print('stuID:%s, stuNM:%s, average_grade:%s' 
                    % (target['stuID'],target['stuNM'],target['average']))
                
                elif exp_sep[1] == 'student':
                    print('stuID:%s, stuNM:%s, gender:%s, age:%s, major:%s, curriculums:%s' 
                    % (target['stuID'],target['stuNM'],target['gender'],target['age'],target['major'],','.join(target['grades'].keys())))

                elif exp_sep[1] == 'cur_num':
                    print('stuID:%s, stuNM:%s, cur_num:%s' 
                    % (target['stuID'],target['stuNM'],len(target['grades'])))
                
                else:
                    print('Invalid Syntax!')
            
        elif key == 'curID':
            target = curriculums[value]
            if exp_sep[1] == 'average':
                
                print('curID:%s, curNM:%s, average_grade:%s' 
                % (target['curID'],target['curNM'],target['average']))

            elif exp_sep[1] == 'curriculum':
                print('curID:%s, curNM:%s, credit:%s, students:%s'
                % (target['curID'],target['curNM'],target['credit'],','.join(target['grades'].keys())))
            
            elif exp_sep[1] == 'stu_num':
 
                print('curID:%s, curNM:%s, stu_num:%s' 
                % (target['curID'],target['curNM'],len(target['grades'])))
            
            else:
                print('Invalid Syntax!')
            
        
        elif key == 'curNM':
            for each in curNM2ID[value]:
                target = curriculums[each]
                if exp_sep[1] == 'average':
                    print('curID:%s, curNM:%s, average_grade:%s' 
                    % (target['curID'],target['curNM'],target['average']))

                elif exp_sep[1] == 'curriculum':
                    print('curID:%s, curNM:%s, credit:%s, students:%s'
                    % (target['curID'],target['curNM'],target['credit'],','.join(target['grades'].keys())))
                
                elif exp_sep[1] == 'stu_num':
 
                    print('curID:%s, curNM:%s, stu_num:%s' 
                    % (target['curID'],target['curNM'],len(target['grades'])))
                
                else:
                    print('Invalid Syntax!')
        else:
            print('Invalid Syntax!')

        
    elif exp_sep[0] == 'revise':
        sources = exp_sep[3].split(',')
        new_val = float(exp_sep[1].split('=')[1])

        for source in sources:
            source = source.split('=')
            key = source[0]
            value = source[1]
            
            if key=='stuID':
                student = value
            else:
                curriculum = value

        grade = students[student]['grades'][curriculum]
        delta = new_val - float(grade)

        stu = students[student]
        cur = curriculums[curriculum]

        delta_average_stu = delta * cur['credit']/stu['credit']
        delta_average_cur = delta/len(cur['grades'])
    
        stu['average'] += delta_average_stu
        cur['average'] += delta_average_cur

        stu['grades'][curriculum] = new_val
        cur['grades'][student] = new_val


        print('Grade of Student %s-%s in %s-%s has been changed to %s' % (student,students[student]['stuNM'],curriculum,curriculums[curriculum]['curNM'],new_val))    
        with open('lab1_students.json','w',encoding='utf-8') as f:
            f.write(json.dumps(students,ensure_ascii=False,cls=int64Encoder))

        with open('lab1_curriculums.json','w',encoding='utf-8') as f:
            f.write(json.dumps(curriculums,ensure_ascii=False,cls=int64Encoder))

        with open('lab1_stuNM2ID.json','w',encoding='utf-8') as f:
            f.write(json.dumps(stuNM2ID,ensure_ascii=False,cls=int64Encoder))
        
        with open('lab1_curNM2ID.json','w',encoding='utf-8') as f:
            f.write(json.dumps(curNM2ID,ensure_ascii=False,cls=int64Encoder))


    # capable of deleting data in students and curriculums, however, cannot remove data in csv records
    elif exp_sep[0] == 'delete':
        sources = exp_sep[3].split(',')

        for source in sources:
            source = source.split('=')
            key = source[0]
            value = source[1]
            
            if key=='stuID':
                student = value
            else:
                curriculum = value
        
        grade = students[student]['grades'][curriculum]
        
        stu = students[student]
        cur = curriculums[curriculum]

        credit = cur['credit']
        average_stu = stu['average']
        average_cur = cur['average']
        credits = stu['credit']

        del stu['grades'][curriculum]
        del cur['grades'][student]

        target = stu
        target['credit'] = credits - credit
        #target['cur_num'] -= 1
        target['average'] = (average_stu * credits - grade * credit)/target['credit']
        
        print('stuID:%s, stuNM:%s, gender:%s, age:%s, major:%s, curriculums:%s' 
            % (target['stuID'],target['stuNM'],target['gender'],target['age'],target['major'],','.join(target['grades'].keys())))

        target = cur
        if len(target['grades']):
            target['average'] = (average_cur * (len(target['grades'])+1) - grade)/(len(target['grades']))
        else:
            target['average'] = 0

        print('curID:%s, curNM:%s, credit:%s, students:%s'
            % (target['curID'],target['curNM'],target['credit'],','.join(target['grades'].keys())))

        with open('lab1_students.json','w',encoding='utf-8') as f:
            f.write(json.dumps(students,ensure_ascii=False,cls=int64Encoder))

        with open('lab1_curriculums.json','w',encoding='utf-8') as f:
            f.write(json.dumps(curriculums,ensure_ascii=False,cls=int64Encoder))

        with open('lab1_stuNM2ID.json','w',encoding='utf-8') as f:
            f.write(json.dumps(stuNM2ID,ensure_ascii=False,cls=int64Encoder))
        
        with open('lab1_curNM2ID.json','w',encoding='utf-8') as f:
            f.write(json.dumps(curNM2ID,ensure_ascii=False,cls=int64Encoder))

    else:
        print('Invalid Syntax!')
    return

if __name__ == "__main__":
    students = {}
    curriculums = {}
    stuNM2ID = {}
    curNM2ID = {}

    # print('LOADING...')
    # students,curriculums,stuNM2ID,curNM2ID = load_data()
    # print('SUCCESS!')

    
    
    print('ADDING...')
    students,curriculums,stuNM2ID,curNM2ID = add_data(students,curriculums,stuNM2ID,curNM2ID)
    print('SUCCESS!')

    while(1):
        print('-'*20)
        query(input('input query：'),students,curriculums,stuNM2ID,curNM2ID)
    
        