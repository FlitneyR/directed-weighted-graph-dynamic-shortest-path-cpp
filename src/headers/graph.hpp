#include <iostream>
#include <string>
#include <vector>

class Graph;
class Node;
class Route;

/// @brief A group of nodes connected by weighted routes
class Graph
{
    friend Graph;
    friend Node;
    friend Route;

    /// @brief A vector storing the names of each node,
    ///     the index being their ID
    std::vector<std::string> names;

    /// @brief A matrix representing the routes connecting each node.
    ///     A cell at [x][y] of value l represents that a route exists
    ///     from x to y with a weight of l.
    ///     A weight less than 0 represents an absence of a route
    std::vector<std::vector<float>> weight_matrix;

    /// @brief A matrix representing the next hop in a route through the graph.
    ///     A cell at [x][y] of value z represents that to get from x
    ///     to y, you must go via z.
    ///     A value of less than 0 represents that no such route exists
    std::vector<std::vector<int>> next_hop_matrix;

public:
    Graph();

    /// @brief Adds a new node to the graph
    ///     Does nothing if the node already exists
    void add_node(const std::string& name);

    /// @brief Adds a new route between two nodes in the graph.
    ///     If either node does not already exist, it is added
    void add_edge(const std::string& from, const std::string& to, const float& weight);

    /// @return A vector representing every node in the graph
    std::vector<Node> nodes() const;

    /// @return A vector representing every route in the graph
    std::vector<Route> routes() const;

    /// @return The number of nodes in the graph
    int node_count() const;

    /// @brief Prints out a representation of the graph
    ///     "A: 2 B, 3 C" represents that a node A exists,
    ///     with a route to B with weight 2, and a route to C,
    ///     with weight 3
    void print() const;

    /// @brief Prints the route from a node to another to std::cout
    void print_route(const std::string& from, const std::string& to) const;
private:

    /// @brief Gets a node representation from it's ID 
    Node get_node(int ID) const;

    /// @brief Gets the ID of the given node.
    ///     Returns -1 if the node does not exist
    int get_ID(const std::string& name) const;
};

/// @brief A node in a graph
class Node
{
    friend Graph;
    friend Node;
    friend Route;

    const int ID;
    const Graph& graph;

    Node(const Graph& graph, int ID);

public:
    /// @return The identifier for this node
    std::string name() const;

    /// @return A vector representing the routes connecting this
    ///     node to other nodes
    std::vector<Route> routes() const;

    /// @brief Prints out a representation of the node and it's routes.
    ///     For example: "D -> B = 3: D -(1)> A -(2)> B" represents that
    ///     to get from costs 3 units, and to get from D to B you must go via A
    ///     and that the step from D to A costs 1 unit and the step from A to B
    ///     costs 2 units
    void print() const;
private:
};

/// @brief A route between two nodes in a graph
class Route
{
    friend Graph;
    friend Node;
    friend Route;

    const Graph& graph;
    const int from_ID, to_ID, via_ID;

public:
    const float weight;

    Route(const Graph& graph, int from_ID, int to_ID, int via_ID, float weight);

    Node from() const;
    Node to() const;
    Node via() const;
private:
};

/// @brief Parses a graph from an input stream.
///     Each line from the input stream represents an edge.
///     "X Y L" represents an edge from a node called X to
///     a node called Y with a weight of L
Graph parse_graph(std::istream* const is);
