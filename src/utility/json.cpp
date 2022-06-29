#include "json.hpp"

namespace Utility
{

namespace Impl
{
    auto JsonRootFactory::operator()(const std::string_view tag) const
        -> Json::Value
    {
        if (!std::filesystem::exists(tag))
            BOOST_THROW_EXCEPTION(InvalidJsonArchive() << JsonArchiveInfo(tag));

        Json::Value root;
        std::ifstream(tag.data()) >> root;
        return root;
    }

} // namespace Impl

const JsonPool jsons;

} // namespace Utility