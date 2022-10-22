// Contains a weight_type repository, which allowes for dynamic dependencies
// scoring. Soultatos Stefanos 2022

#ifndef WEIGHTS_WEIGHT_REPO_HPP
#define WEIGHTS_WEIGHT_REPO_HPP

#include "misc/heterogeneous.hpp"

#include <string>
#include <string_view>

namespace weights
{

// Manages a table of strings, provides an interface of string views.
class weight_repo
{
public:
    using dependency_type = std::string_view;
    using weight_type = int;
    using hash_table = misc::unordered_string_map< weight_type >;

    explicit weight_repo(hash_table table = hash_table());

    auto get_weight(dependency_type dependency) const -> weight_type;

    // Precondition: the dependency must have been assigned on construction.
    auto set_weight(dependency_type dependency, weight_type score) -> void;

    auto begin() -> auto { return std::begin(m_map); }
    auto begin() const -> auto { return std::begin(m_map); }
    auto cbegin() const -> auto { return std::cbegin(m_map); }

    auto end() -> auto { return std::end(m_map); }
    auto end() const -> auto { return std::end(m_map); }
    auto cend() const -> auto { return std::cend(m_map); }

    auto operator==(const weight_repo&) const -> bool = default;
    auto operator!=(const weight_repo&) const -> bool = default;

private:
    hash_table m_map;
};

} // namespace weights

#endif // WEIGHTS_WEIGHT_REPO_HPP
