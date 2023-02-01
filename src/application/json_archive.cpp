#include "json_archive.hpp"

#include <boost/exception/all.hpp>
#include <cassert>
#include <fstream>

namespace application
{

auto json_archive::archived(str_ref_type file) const -> bool
{
    return m_map.contains(file);
}

auto json_archive::get(str_ref_type file) const -> const json_type&
{
    auto iter = m_map.find(file);
    assert(iter != std::cend(m_map));
    return iter->second;
}

auto json_archive::get(str_ref_type file) -> json_type&
{
    return const_cast< json_type& >(std::as_const(*this).get(file));
}

auto json_archive::put(str_type file, json_type&& json) -> void
{
    m_map[std::move(file)] = std::move(json);
}

auto import(const file_path_t& from) -> json_t
{
    if (!std::filesystem::exists(from))
        BOOST_THROW_EXCEPTION(
            invalid_file_path() << file_path_info(from.string()));

    try
    {
        json_t root;
        auto stream = std::ifstream { from };
        assert(stream.good());
        stream >> root;
        return root;
    }
    catch (const Json::RuntimeError&)
    {
        BOOST_THROW_EXCEPTION(
            invalid_file_path() << file_path_info(from.string()));
    }
}

auto import(json_archive& archive, const file_path_t& from) -> void
{
    archive.put(from.string(), import(from));
}

auto dump(const json_t& json, const file_path_t& to) -> void
{
    if (!std::filesystem::exists(to))
        BOOST_THROW_EXCEPTION(
            invalid_file_path() << file_path_info(to.string()));

    try
    {
        auto stream = std::ofstream { to };
        assert(stream.good());
        stream << json;
    }
    catch (const Json::RuntimeError&)
    {
        BOOST_THROW_EXCEPTION(
            invalid_file_path() << file_path_info(to.string()));
    }
}

auto dump(const json_archive& archive, const file_path_t& to) -> void
{
    dump(archive.get(to.c_str()), to);
}

json_archive jsons;

} // namespace application