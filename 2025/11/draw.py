import graphviz
import os

def parse_adjacency_list(text_input):
    """
    Parses a string containing an adjacency list into a Python dictionary.
    (Same parser function as provided previously)
    """
    adjacency_dict = {}
    lines = text_input.strip().split('\n')
    for line in lines:
        if line.strip():
            try:
                source, targets_str = line.split(':', 1)
            except ValueError:
                print(f"Skipping line due to invalid format (missing colon): {line}")
                continue
            source_node = source.strip()
            targets_list = [target.strip() for target in targets_str.split() if target.strip()]
            adjacency_dict[source_node] = targets_list
    return adjacency_dict

def visualize_graph_with_graphviz(adjacency_dict, graph_name="MyGraph", format="png", view=True):
    """
    Visualizes a graph represented by an adjacency dictionary using Graphviz DOT.

    Args:
        adjacency_dict (dict): The parsed graph in adjacency list format.
        graph_name (str): The name for the graph (used for output file).
        format (str): The output format (e.g., 'png', 'pdf', 'svg').
        view (bool): If True, attempts to open the generated graph file.
    """
    
    # Create a directed graph object
    # 'comment' is just a description within the DOT file
    # 'graph_attr' and 'node_attr' allow global styling
    dot = graphviz.Digraph(
        name=graph_name, 
        comment='Parsed Adjacency List Graph',
        engine='dot', # 'dot' is for hierarchical layouts
        graph_attr={'rankdir': 'LR', 'splines': 'spline', 'overlap': 'false', 'nodesep': '0.8', 'ranksep': '0.7'},
        node_attr={'shape': 'box', 'style': 'filled', 'fillcolor': 'lightblue', 'fontname': 'Helvetica'},
        edge_attr={'color': 'gray', 'arrowhead': 'vee', 'arrowsize': '1.2'}
    )


    special_colors = {
        'svr': 'red',     # Color the starting node red
        'out': 'green',   # Color the ending node green
        'dac': 'yellow',   # Color a key node yellow
        'fft': 'blue'   # Color a key node yellow
    }

    # Add nodes to the graph
    # First, collect all unique nodes (both sources and targets)
    all_nodes = set()
    for source, targets in adjacency_dict.items():
        all_nodes.add(source)
        for target in targets:
            all_nodes.add(target)

   # Add nodes to the graph, checking for special colors
    for node in sorted(list(all_nodes)):
        if node in special_colors:
            # Overwrite the global 'fillcolor' for this specific node
            dot.node(node, fillcolor=special_colors[node]) # <-- CHANGED
        else:
            # Use the global default color
            dot.node(node) 

    # Add edges to the graph
    for source, targets in adjacency_dict.items():
        for target in targets:
            dot.edge(source, target)

    # Define the output file path
    output_filename = f"{graph_name}.{format}"
    
    # Render the graph
    # 'filename' is the name of the DOT file and the output file base name
    # 'format' is the output image format
    # 'view' tells graphviz to open the file after generation
    try:
        dot.render(filename=graph_name, format=format, view=view, cleanup=True)
        print(f"Graph '{graph_name}' rendered successfully to {output_filename}")
        if view:
            print(f"Attempting to open {output_filename}")
    except graphviz.backend.ExecutableNotFound:
        print("Error: Graphviz 'dot' executable not found.")
        print("Please ensure Graphviz is installed and its 'bin' directory is in your system's PATH.")
        print("See: https://graphviz.org/download/ for installation instructions.")
    except Exception as e:
        print(f"An error occurred during rendering: {e}")

# --- Main Execution ---
if __name__ == "__main__":
    input_data = open(0).read() 
    print("Parsing input data...")
    parsed_graph = parse_adjacency_list(input_data)
    
    print("\nVisualizing graph with Graphviz DOT...")
    # You can change format='pdf', format='svg' as needed
    visualize_graph_with_graphviz(parsed_graph, graph_name="AdjacencyGraph", format="png", view=True)
    
    # Optionally, clean up the generated .dot file
    # if os.path.exists("AdjacencyGraph.dot"):
    #     os.remove("AdjacencyGraph.dot")
