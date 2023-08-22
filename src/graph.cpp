#include <graph.hpp>

Graph::Graph()
{}

void Graph::add_node(const std::string& name)
{
    // If the node already exists, do nothing
    if (get_ID(name) >= 0) return;
    
    // Add a row in the weight matrix for this node with no connections
    std::vector<float> new_weight_matrix_row(node_count(), -1);
    weight_matrix.push_back(std::move(new_weight_matrix_row));
    
    // Add a row in the next hop matrix for this node with no connections
    std::vector<int> new_next_hop_matrix_row(node_count(), -1);
    next_hop_matrix.push_back(std::move(new_next_hop_matrix_row));

    // Add a column in the weight matrix for this node with no connections
    for (std::vector<float>& row : weight_matrix) row.push_back(-1);

    // Add a column in the next hop matrix for this node with no connections
    for (std::vector<int>& row : next_hop_matrix) row.push_back(-1);

    // Add this nodes name to the names list
    names.push_back(name);

    int ID = get_ID(name);

    // Add into the matrix that you can go from a node to itself by doing nothing
    weight_matrix[ID][ID] = 0;
    next_hop_matrix[ID][ID] = ID;
}

void Graph::add_edge(
    const std::string& from,
    const std::string& to,
    const float& weight
) {
    int from_ID = get_ID(from);
    int to_ID = get_ID(to);

    // if either node does not exist, add it
    if (from_ID < 0)
    {
        add_node(from);
        from_ID = get_ID(from);
    }

    if (to_ID < 0)
    {
        add_node(to);
        to_ID = get_ID(to);
    }

    // if there is already a shorter route, ignore this edge
    // if this edge is the same length as an existing route,
    // still overwrite it, it will probably make the routes simpler
    float& current_weight = weight_matrix[from_ID][to_ID];
    if (current_weight >= 0 && current_weight < weight) return;

    // add this edge as a new route between the two nodes
    current_weight = weight;
    next_hop_matrix[from_ID][to_ID] = to_ID;

    // check if this edge makes any routes shorter
    int _node_count = node_count();

    // for every distinct route from one node to another node
    for (int start = 0; start < _node_count; start++)
    for (int end = 0; end < _node_count; end++)
    if (start != end)
    {
        // get the distance from the start of this route to this new edge
        float first_part_weight = weight_matrix[start][from_ID];
        // get the distance from the end of this edge to end of this route
        float second_part_weight = weight_matrix[to_ID][end];

        // if either route does not exist
        if (first_part_weight < 0) continue;
        if (second_part_weight < 0) continue;

        // find the total distance of the new route through this edge
        float weight_via_this_edge = first_part_weight + weight + second_part_weight;
        // find the length of the current route
        float current_shortest_route = weight_matrix[start][end];

        // if there is no current route, or the new route is shorter
        if (current_shortest_route < 0 || weight_via_this_edge < current_shortest_route)
        {
            // update the distance for this route
            weight_matrix[start][end] = weight_via_this_edge;

            // set the next hop for this route to be the next hop to get to this edge
            int& next_hop = next_hop_matrix[start][end];
            // if the new edge starts at the start of this route, the next step is to the end of the new edge
            if (from_ID == start) next_hop = next_hop_matrix[start][to_ID];
            // otherwise the next step is to the start of the new edge
            else next_hop = next_hop_matrix[start][from_ID];
        }
    }
}

Node Graph::get_node(int ID) const
{ return Node(*this, ID); }

std::vector<Node> Graph::nodes() const
{
    std::vector<Node> result;

    for (int i = 0; i < node_count(); i++)
        result.push_back(Node(*this, i));

    return result;
}

std::vector<Route> Graph::routes() const
{
    std::vector<Route> result;

    for (const Node& node : nodes())
    for (const Route& edge : node.routes())
        result.push_back(edge);

    return result;
}

int Graph::node_count() const
{
    return names.size();
}

void Graph::print() const
{
    for (auto& node : nodes())
        node.print();
}

void Graph::print_route(const std::string& from, const std::string& to) const
{
    int from_ID = get_ID(from);
    int to_ID = get_ID(to);

    if (from_ID < 0)
    {
        std::cerr << "Did not recognise node name: " << from << std::endl;
        return;
    }

    if (to_ID < 0)
    {
        std::cerr << "Did not recognise node name: " << to << std::endl;
        return;
    }

    if (weight_matrix[from_ID][to_ID] < 0)
    {
        std::cerr << "No route exists between " << from << " and " << to << std::endl;
        return;
    }

    std::cout << from << " -> " << to << " = " << weight_matrix[from_ID][to_ID] << ": ";

    int node_ID = from_ID;
    while(true)
    {
        int next_ID = next_hop_matrix[node_ID][to_ID];

        std::cout << get_node(node_ID).name();
        if (node_ID == to_ID)
        {
            std::cout << std::endl;
            break;
        }
        else std::cout << " -(" << weight_matrix[node_ID][next_ID] << ")> ";

        node_ID = next_ID;
    }
}

int Graph::get_ID(const std::string& name) const
{
    int _node_count = node_count();
    for (int i = 0; i < _node_count; i++)
        if (names[i] == name) return i;

    return -1;
}

Node::Node(const Graph& graph, int ID) :
    ID(ID),
    graph(graph)
{}

std::string Node::name() const
{
    return graph.names[ID];
}

std::vector<Route> Node::routes() const
{
    std::vector<Route> result;

    int from = ID;

    int _node_count = graph.node_count();
    for (int to = 0; to < _node_count; to++)
    {
        float weight = graph.weight_matrix[from][to];
        if (weight < 0) continue;
        int via = graph.next_hop_matrix[from][to];
        result.push_back(Route(graph, from, to, via, weight));
    }

    return result;
}

void Node::print() const
{
    std::cout << name() << ":" << std::endl;

    std::vector<Route> _routes = routes();
    for (auto iter = _routes.begin(); iter != _routes.end(); iter++)
    {
        if (iter->to_ID == ID) continue;

        std::cout << "\t" << iter->to().name() << " " << iter->weight;

        if (iter->via_ID != iter->to_ID)
            std::cout << " " << iter->via().name();

        std::cout << std::endl;
    }
}

Route::Route(const Graph& graph, int from_ID, int to_ID, int via_ID, float weight) :
    graph(graph),
    from_ID(from_ID),
    to_ID(to_ID),
    via_ID(via_ID),
    weight(weight)
{}

Node Route::from() const
{ return graph.get_node(from_ID); }

Node Route::to() const
{ return graph.get_node(to_ID); }

Node Route::via() const
{ return graph.get_node(via_ID); }

Graph parse_graph(std::istream* const is)
{
    Graph graph;

    while (true)
    {
        std::string from;
        std::string to;
        float weight;
        *is >> from >> to >> weight;

        if (is->eof()) break;
        if (is->fail())
            throw std::runtime_error("Failed to parse graph");

        graph.add_edge(from, to, weight);
    }

    return graph;
}
