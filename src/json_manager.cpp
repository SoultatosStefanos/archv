#include "json_manager.hpp"

#include <filesystem>
#include <fstream>

namespace Utility
{

namespace // error info
{
    using JsonArchiveInfo =
        boost::error_info<struct JsonArchiveTag, const std::string_view>;

} // namespace

namespace
{
    inline auto make_root(const JsonManager::FilePath from)
    {
        assert(std::filesystem::exists(from));

        JsonManager::JsonVal root;
        std::ifstream(from.data()) >> root;

        return root;
    }

} // namespace

auto JsonManager::croot(const FilePath from) const -> const JsonVal&
{
    if (!std::filesystem::exists(from))
        BOOST_THROW_EXCEPTION(InvalidJsonArchive() << JsonArchiveInfo(from));

    return cache.contains(from) ? cache.at(from)
                                : cache[from] = make_root(from);
}

auto JsonManager::root(const FilePath from) -> JsonVal&
{
    return const_cast<JsonVal&>(std::as_const(*this).croot(from));
}

} // namespace Utility