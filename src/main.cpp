#include <iostream>
#include <fstream>
#include <string>

#include "graph.hpp"

int main(const int argc, const char** const argv)
{
    Graph graph;

    if (argc > 4)
    {
        std::cerr << "Invalid arguments!" << std::endl;
        std::cerr << "You must supply either:" << std::endl;
        std::cerr << "\t(No arguments)," << std::endl;
        std::cerr << "\tFrom(a node name) To(a node name)," << std::endl;
        std::cerr << "\tGraph(a path to a text file describing a graph)," << std::endl;
        std::cerr << "\tor Graph From To" << std::endl;
        return 1;
    }

    if (argc == 2 || argc == 4)
    {
        std::ifstream file(argv[1]);
        graph = parse_graph(&file);
    }
    else graph = parse_graph(&std::cin);

    if (argc == 3 || argc == 4)
    {
        std::string from(argv[argc == 3 ? 1 : 2]), to(argv[argc == 3 ? 2 : 3]);
        graph.print_route(from, to);
    }
    else graph.print();
    
    return 0;
}
