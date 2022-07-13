#include "flyweight.hpp"

namespace utility
{

namespace detail
{
    auto json_root_factory::operator()(const std::string_view tag) const
        -> Json::Value
    {
        if (!std::filesystem::exists(tag))
            BOOST_THROW_EXCEPTION(invalid_json_archive()
                                  << json_archive_info(tag));

        Json::Value root;
        std::ifstream(tag.data()) >> root;
        return root;
    }

} // namespace detail

const json_pool jsons;

} // namespace utility