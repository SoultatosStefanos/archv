// Contains an architecture graph generation function, from a .json archive.
// Soultatos Stefanos 2022

#ifndef ARCHITECTURE_GRAPH_GENERATION_HPP
#define ARCHITECTURE_GRAPH_GENERATION_HPP

#include "graph.hpp"

#include <jsoncpp/json/json.h>
#include <stdexcept>

namespace Architecture
{

struct InvalidJsonArchive : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

void generate_graph(Graph& g, const Json::Value& root);

} // namespace Architecture

#endif // ARCHITECTURE_GRAPH_GENERATION_HPP
