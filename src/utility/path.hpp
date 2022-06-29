// Contains utilitites for file paths.
// Soultatos Stefanos 2022

#ifndef UTILITY_PATH_HPP
#define UTILITY_PATH_HPP

#include <experimental/source_location>
#include <filesystem>

namespace Utility
{

// Resolve path from a source location, to a realtive target location.
// Useful for out of source builds.
inline auto resolve_path(const std::string_view to,
                         const std::experimental::source_location from =
                             std::experimental::source_location::current())
{
    return std::filesystem::absolute(from.file_name()).parent_path() /
           std::filesystem::path{to};
}

} // namespace Utility

#endif // UTILITY_PATH_HPP
