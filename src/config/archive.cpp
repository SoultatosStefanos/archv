#include "archive.hpp"

#include <filesystem>
#include <fstream>
#include <ranges>

namespace config
{

auto archive::contains(file_name_type fname) const -> bool
{
    return m_roots.contains(fname);
}

auto archive::operator[](file_name_type fname) -> json_root_type&
{
    return m_roots[fname];
}

auto archive::at(file_name_type fname) const -> const json_root_type&
{
    assert(contains(fname));
    return m_roots.at(fname);
}

namespace
{
    inline auto verify(archive::file_name_type fname) -> void
    {
        if (!std::filesystem::exists(fname))
            BOOST_THROW_EXCEPTION(invalid_json_file() << json_file_info(fname));
    }

} // namespace

auto read_archive(archive::file_name_type fname) -> void
{
    verify(fname);
    std::ifstream(fname.data()) >> archive::get()[fname];
}

auto read_archive_once(archive::file_name_type fname) -> void
{
    if (!archive::get().contains(fname))
        read_archive(fname);
}

auto write_archive(archive::file_name_type fname) -> void
{
    verify(fname);
    std::ofstream(fname.data()) << archive::get().at(fname);
}

auto write_all_archives() -> void
{
    using std::ranges::views::keys;

    for (auto fname : keys(archive::get()))
        write_archive(fname);
}

} // namespace config