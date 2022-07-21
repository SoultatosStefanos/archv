#ifndef CONFIG_ARCH_DESERIALIZATION_HPP
#define CONFIG_ARCH_DESERIALIZATION_HPP

#include "dependencies/all.hpp"
#include "symbols/all.hpp"

#include <jsoncpp/json/json.h>
#include <unordered_map>
#include <utility>

namespace config
{

auto derialize_symbols(const Json::Value& root) -> symbols::symbol_table;

auto deserialize_dependencies(const Json::Value& root,
                              const symbols::symbol_table& st)
    -> std::pair<dependencies::graph, dependencies::vertex_table>;

} // namespace config

#endif // CONFIG_ARCH_DESERIALIZATION_HPP
