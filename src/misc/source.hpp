// Contains utilitites for source file paths.
// Soultatos Stefanos 2022

#ifndef MISC_SOURCE_HPP
#define MISC_SOURCE_HPP

#include <experimental/source_location>
#include <filesystem>

namespace misc
{

// Resolve path from a source location, to a realtive target location.
// Useful for out of source builds.
inline auto resolve_source_path(
    const std::filesystem::path& to,
    const std::experimental::source_location& from
    = std::experimental::source_location::current()) -> std::filesystem::path
{
    return std::filesystem::absolute(from.file_name()).parent_path() / to;
}

} // namespace misc

#endif // MISC_SOURCE_HPP
