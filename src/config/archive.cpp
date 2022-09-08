#include "archive.hpp"

#include <filesystem>
#include <fstream>
#include <ranges>

namespace config
{

namespace
{
    auto read_archive(archive::file_name_type fname)
    {
        archive::json_root_type root;
        std::ifstream(fname.data()) >> root;
        return root;
    }

    auto write_archive(archive::file_name_type fname) -> void
    {
        std::ofstream(fname.data()) << archive::get().at(fname);
    }

} // namespace

archive::~archive()
{
    for (auto fname : std::ranges::views::keys(m_roots))
        write_archive(fname);

    m_roots.clear();
}

auto archive::at(file_name_type fname) const -> const json_root_type&
{
    return m_roots.contains(fname) ? m_roots.at(fname)
                                   : m_roots[fname] = read_archive(fname);
}

auto archive::at(file_name_type fname) -> json_root_type&
{
    return const_cast< json_root_type& >(std::as_const(*this).at(fname));
}

} // namespace config