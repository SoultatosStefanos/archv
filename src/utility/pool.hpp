// Contains a generic resource pool object.
// Soultatos Stefanos 2022

#ifndef UTILITY_POOL_HPP
#define UTILITY_POOL_HPP

#include <concepts>
#include <type_traits>
#include <unordered_map>

namespace utility
{

namespace detail
{
    struct constructor
    {
        template <typename Resource, typename Tag>
        requires std::constructible_from<Resource, Tag>
        constexpr auto operator()(const Tag& tag) const
            noexcept(std::is_nothrow_constructible_v<Resource, Tag>)
        {
            return Resource(tag);
        }
    };

} // namespace detail

// A general purpose resource pool for tagged resources.
template <typename Resource,
          typename Tag,
          typename Factory = detail::constructor,
          typename AssociativeContainer = std::unordered_map<Tag, Resource>>
class pool
{
public:
    static_assert(std::is_invocable_r_v<Resource, Factory, std::decay_t<Tag>>);
    static_assert(std::is_same_v<typename AssociativeContainer::key_type, Tag>);
    static_assert(
        std::is_same_v<typename AssociativeContainer::mapped_type, Resource>);

    auto get(const Tag& tag) const -> const Resource&
    {
        return m_data.contains(tag) ? m_data.at(tag)
                                    : m_data[tag] = m_factory(tag);
    }

    auto get(const Tag& tag) -> Resource&
    {
        return const_cast<Resource&>(std::as_const(*this).get(tag));
    }

private:
    Factory m_factory;
    mutable AssociativeContainer m_data;
};

} // namespace utility

#endif // UTILITY_POOL_HPP
