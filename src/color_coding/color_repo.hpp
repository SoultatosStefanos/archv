// Contains the repository of the color coding subsystem.
// Soultatos Stefanos 2022

#ifndef COLOR_CODING_COLOR_REPO_HPP
#define COLOR_CODING_COLOR_REPO_HPP

#include "misc/heterogeneous.hpp" // for unordered_string_map

#include <array>       // for array
#include <optional>    // for optional
#include <string_view> // for string_view

namespace color_coding
{

struct color_code
{
    using rgba_type = std::array< float, 4 >;

    rgba_type rgba;
    bool active { true };

    constexpr auto operator==(const color_code&) const -> bool = default;
    constexpr auto operator!=(const color_code&) const -> bool = default;
};

class color_repo
{
public:
    using dependency_type = std::string_view;
    using hash_table = misc::unordered_string_map< color_code >;

    explicit color_repo(hash_table map = hash_table());

    // Precondition: the dependency must have been assigned on construction.

    auto get_color(dependency_type dep) const -> color_code;
    auto set_color(dependency_type dep, color_code rgba) -> void;

    auto begin() -> auto { return std::begin(m_map); }
    auto begin() const -> auto { return std::begin(m_map); }
    auto cbegin() const -> auto { return std::cbegin(m_map); }

    auto end() -> auto { return std::end(m_map); }
    auto end() const -> auto { return std::end(m_map); }
    auto cend() const -> auto { return std::cend(m_map); }

    auto operator==(const color_repo&) const -> bool = default;
    auto operator!=(const color_repo&) const -> bool = default;

private:
    hash_table m_map;
};

} // namespace color_coding

#endif // COLOR_CODING_COLOR_REPO_HPP
