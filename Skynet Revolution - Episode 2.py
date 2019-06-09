import sys
import copy

#can the virus get there before we can cut all if its gateway edges
#if distance to node is less than the number of cuts to make a node safe


def zero_out(adj_matrix, zeroed_nodes):
    for j in zeroed_nodes:
        for i in range(len(adj_matrix)):
            adj_matrix[j][i] = -1
            adj_matrix[i][j]= -1
    return adj_matrix

def cut_graph(graph, node1, node2, zeroed_nodes):
    del graph[node1][graph[node1].index(node2)]
    del graph[node2][graph[node2].index(node1)]
    
    if not graph[node1]:
        del graph[node1]
        for i in graph:
            if node1 in graph[i]:
                del graph[i][node1]
        zeroed_nodes.append(node1)
            
    if not graph[node2]:
        del graph[node2]
        for i in graph:
            if node2 in graph[i]:
                del graph[i][node2]
        zeroed_nodes.append(node2)
    
    
    return graph

def create_adj_matrix(level, adj_matrix):
    
    cont = True
    while cont == True:
        for y_value, i in enumerate(adj_matrix):
            for x_value,node in enumerate(i):
                if node == level:
                    for second_value, second_node in enumerate(adj_matrix[x_value]):
                        if second_node == 1:
                            if adj_matrix[y_value][second_value] == 0:
                                pass
                            elif adj_matrix[y_value][second_value] == None:
                                adj_matrix[y_value][second_value] = level+1

                                
        for i in adj_matrix:
            if None in i:
                cont = True
                level+=1
                break
        else:
            cont = False
    
    return adj_matrix
    
def create_node_weights(graph, gateways, cost_graph):
    
    for i in graph:
        for j in gateways:
            if j in graph[i]:
                if i in cost_graph:
                    cost_graph[i] += 1
                else:
                    cost_graph[i] = 1
    return cost_graph
    
def create_frame(graph, adj_matrix):        
    for i in graph:
        for node in graph[i]:
            adj_matrix[node][i] = 1
            adj_matrix[i][node] = 1
        adj_matrix[i][i] = 0

    return adj_matrix

def create_priority_queue(si, graph, cost_graph, adj_matrix, gateways, queue):
    for i in cost_graph:
            for j in graph[i]:
                if j in gateways:
                    if cost_graph[i] in queue:
                        queue[cost_graph[i]].append((i,j))
                    else:
                        queue[cost_graph[i]] = [(i,j)]
    return queue


def djik(graph, adj_matrix, cost_graph, gateways, si, goal):
    easy_visit = {si: 0}
    if si in cost_graph:
        visited = {si: cost_graph[si]}
    else:
        visited = {si: 0}
    path = {}
    ret_path = []
    q = copy.deepcopy(graph)
    
    while q:
        min_node = None
        for node in q:
            if node in gateways:
                continue
            if node in visited:
                if min_node == None:
                    min_node = node
                elif visited[node] < visited[min_node]:
                    min_node = node
                    
                    
        if min_node is None:
            break
        
        del q[min_node]
        current_weight = visited[min_node]
        basic_weight = easy_visit[min_node]
        
        for edge in graph[min_node]:
            for checks in graph[edge]:
                if checks in gateways:
                    weight = current_weight  + adj_matrix[min_node][edge] +cost_graph[edge]
                    break
            else:
                weight = current_weight + adj_matrix[min_node][edge]
            if edge not in visited or weight < visited[edge]:
                visited[edge] = weight
                path[edge] = min_node
                easy_visit[edge] = basic_weight + adj_matrix[min_node][edge]
    current = goal
    while current != si:
        ret_path.append(current)
        current = path[current]
    ret_path.append(si)
    ret_path.reverse()
    return visited, ret_path, easy_visit
                

graph = {}
cost_graph = {}
gateways = []

n, l, e = [int(i) for i in input().split()]
adj_matrix, gate_matrix, path = [[None for i in range(n)] for j in range(n)], [[None for i in range(n)] for j in range(n)],[[None for i in range(n)] for j in range(n)]

for i in range(l):
    n1, n2 = [int(j) for j in input().split()]
    if n1 in graph:
        graph[n1].append(n2)
    else:
        graph[n1] = [n2]
    if n2 in graph:
        graph[n2].append(n1)
    else:
        graph[n2] = [n1]
        
adj_matrix = create_frame(graph, adj_matrix)
adj_matrix = create_adj_matrix(1, adj_matrix)
    
for i in range(e):
    ei = int(input()) 
    gateways.append(ei)

cost_graph = create_node_weights(graph, gateways, cost_graph)   

first_turn = True
zeroed_nodes = []
visited, path = {}, {}
while True:
    si = int(input()) 
    if first_turn:
        first_turn = False
    else:
        cost_graph = {}
        adj_matrix = [[None for i in range(n)] for j in range(n)]
        adj_matrix = create_frame(graph, adj_matrix)
        adj_matrix = zero_out(adj_matrix, zeroed_nodes)
        adj_matrix = create_adj_matrix(1, adj_matrix)
        cost_graph = create_node_weights(graph, gateways, cost_graph)
    '''
    ERROR
    for i in graph:
        print(i, graph[i], file=sys.stderr)       
    for i in cost_graph:
        print(i, cost_graph[i], file=sys.stderr)
    for i in adj_matrix:
        print(i, file=sys.stderr)    
    queue = {}
    queue = create_priority_queue(si, graph, cost_graph, adj_matrix, gateways, queue)
    
    score = sys.maxsize
    for i in queue[min(queue, key=queue.get)]:
        print(i, queue, min(queue, key=queue.get), queue[min(queue, key=queue.get)], file=sys.stderr)
        x,y = i
        temp_score = adj_matrix[x][si]
        if temp_score < score:
            score = temp_score
            final_x, final_y = x,y
    '''
    node_paths = {}

    for i in gateways:
        visited,path,easy_visit = djik(graph, adj_matrix, cost_graph,gateways, si, i)
    
    for i in gateways:
        for j in graph[i]:
            if j in node_paths:
                node_paths[j].append(i)
            else:
                node_paths[j] = [i]
        del visited[i]
    
    #print(node_paths, file=sys.stderr)
    print(visited,  file=sys.stderr)
    print(easy_visit, file=sys.stderr)
    biggest = 0
    for i in visited:
        #print(visited[i], adj_matrix[si][i], i, file=sys.stderr)
        if visited[i] == 1 and adj_matrix[si][i] == 0:
            winner = i
            break
        else:
            if visited[i] > easy_visit[i]:
                #print(i, file=sys.stderr)#node_paths[i][0], file=sys.stderr)
                large = visited[i] - easy_visit[i]
                print(str(i)+":", "Weighted:", visited[i], "Unweighted:", easy_visit[i],  "Score:", large, file=sys.stderr)
                if large > easy_visit[i]:
                    winner = i
                    break
                if large > biggest:
                    biggest = large
                    winner = i
                #print(visited[i] - adj_matrix[si][i], file=sys.stderr)
    print("Largest is:", winner, "By:", biggest, file=sys.stderr)
    for i in visited:
        pass
    print(winner,    node_paths[winner][0])  
    #print(final_x, final_y)
    graph = cut_graph(graph, winner, node_paths[winner][0], zeroed_nodes)
    for i in zeroed_nodes:
        if i in gateways:
            del gateways[gateways.index(i)]
    #the cost graph needs dykstras run on it
    
    