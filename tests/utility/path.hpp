// Contains utilitites for file paths.
// Soultatos Stefanos 2022

#ifndef UTILITY_PATH_HPP
#define UTILITY_PATH_HPP

#include <experimental/source_location>
#include <filesystem>

namespace utility
{

// Resolve path from a source location, to a realtive target location.
// Useful for out of source builds.
inline auto resolve_path(
    std::string_view to,
    const std::experimental::source_location& from
    = std::experimental::source_location::current())
{
    return std::filesystem::absolute(from.file_name()).parent_path()
        / std::filesystem::path { to };
}

} // namespace utility

#endif // UTILITY_PATH_HPP
