import nfa
import networkx as nx
import matplotlib.pyplot as plt

def create_nfa(regex):
    N = nfa.NFA(regex)
    N.build_NFA()
    return N
    
def create_digraph(regex):
    # create a digraph
    G = nx.DiGraph()

    # create an NFA
    N = create_nfa(regex)

    # retireve nodes from the NFA
    states = N.set_of_states
    nodes = []
    nodes.append("start")
    for state in states:
        nodes.append(state)

    # retrieve edges from the NFA
    map_edges = N.transition_function
    edges = []
    edges.append(("start", 0))
    for (state, ch) in map_edges:
        for frontier_state in map_edges[(state, ch)]:
            edges.append((state, frontier_state))

    edge_labels = {}
    for (state, ch) in map_edges:
        for frontier_state in map_edges[(state, ch)]:
            edge_labels[(state, frontier_state)] = ch

    # build the digraph
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)
    pos = nx.random_layout(G)
    node_color = []
    for node in nodes:
        if node == "start":
            node_color.append("white")
        elif node in N.accepting_states:
            node_color.append("mistyrose")
        else:
            node_color.append("skyblue")


    plt.figure(figsize=(10, 9))
    nx.draw_networkx(G, pos, arrows=True, node_color=node_color, width=0.5)
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_color='red')
