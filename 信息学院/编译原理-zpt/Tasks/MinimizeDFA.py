from collections import defaultdict
delta = {
    '0':{
        'a':'1',
        'b':'2'
    },
    '1':{
        'a':'1',
        'b':'3'
    },
    '2':{
        'a':'1',
        'b':'2'
    },
    '3':{
        'a':'1',
        'b':'4'
    },
    '4':{
        'a':'1',
        'b':'2'
    }
}

dfa = ({'0','1','2','3','4'},('a','b'),delta,'0',{'4'})
states = dfa[0]
finals = dfa[-1]
words = dfa[1]
union = [states - finals, finals]

def isIn(keys, union):
    for unit in union:
        if keys.issubset(unit):
            return True

    return False

def minimize_DFA(union):
    union_next = []
    for group in union:
        if len(group) < 2:
            union_next.append(group)
            continue
        next_states = defaultdict(set)
        next_states_group = set()
        excluded = set()

        for state in group:
            for word in words:
                next_state = delta[state][word]
                next_states[next_state].add(state)
                next_states_group.add(next_state)

        if not isIn(next_states_group, union):
            for v in next_states_group - group:
                union_next.append(next_states[v])
                excluded = excluded | next_states[v]
            # for k,v in next_states.items():
            #     union_next.append(v)
            #     excluded = excluded | v
        if group - excluded:
            union_next.append(group - excluded)

    if len(union) == len(union_next):
        return union_next
    else:
        return minimize_DFA(union_next)

print(minimize_DFA(union))