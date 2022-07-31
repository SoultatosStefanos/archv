#include "json_archive.hpp"

#include <filesystem>
#include <fstream>

namespace config
{

namespace
{
    auto read_root(std::string_view fname) -> Json::Value
    {
        if (!std::filesystem::exists(fname))
            BOOST_THROW_EXCEPTION(invalid_json_file() << json_file_info(fname));

        Json::Value root;
        std::ifstream(fname.data()) >> root;
        return root;
    }

} // namespace

auto json_archive::at(std::string_view fname) const -> const Json::Value&
{
    return m_roots.contains(fname) ? m_roots.at(fname)
                                   : m_roots[fname] = read_root(fname);
}

} // namespace config