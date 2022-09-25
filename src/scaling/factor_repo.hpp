// Contains a factor repository class.
// Soultatos Stefanos 2022

#ifndef SCALING_FACTOR_REPO_HPP
#define SCALING_FACTOR_REPO_HPP

#include "factor.hpp"

#include <misc/heterogeneous.hpp>
#include <string_view>

namespace scaling
{

class factor_repo
{
public:
    using tag_type = std::string_view;
    using hash_table = misc::unordered_string_map< factor >;

    explicit factor_repo(hash_table factors = hash_table());

    // Precondition: the repo must have been constructed with the tag.

    auto get_factor(tag_type tag) const -> const factor&;
    auto get_factor(tag_type tag) -> factor&;

    auto begin() -> auto { return std::begin(m_map); }
    auto begin() const -> auto { return std::begin(m_map); }
    auto cbegin() const -> auto { return std::cbegin(m_map); }

    auto end() -> auto { return std::end(m_map); }
    auto end() const -> auto { return std::end(m_map); }
    auto cend() const -> auto { return std::cend(m_map); }

    auto operator==(const factor_repo&) const -> bool = default;
    auto operator!=(const factor_repo&) const -> bool = default;

private:
    hash_table m_map;
};

} // namespace scaling

#endif // SCALING_FACTOR_REPO_HPP
