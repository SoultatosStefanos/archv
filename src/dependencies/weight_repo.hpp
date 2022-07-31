// Contains a weight repository, which allowes for dynamic dependencies scoring.
// Soultatos Stefanos 2022

#ifndef DEPENDENCIES_WEIGHT_REPO_HPP
#define DEPENDENCIES_WEIGHT_REPO_HPP

#include <string>
#include <unordered_map>

namespace dependencies
{

/***********************************************************
 * Weight Repo                                             *
 ***********************************************************/

class weight_repo
{
public:
    using dependency_type = std::string;
    using weight = int;
    using hash_table = std::unordered_map<dependency_type, weight>;

    weight_repo() = default;
    explicit weight_repo(hash_table table);

    auto get_weight(const dependency_type& type) const -> weight;
    void set_weight(const dependency_type& type, weight score);

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

} // namespace dependencies

#endif // DEPENDENCIES_WEIGHT_REPO_HPP
