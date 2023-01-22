#include "binary_generator.hpp"

#include "misc/algorithm.hpp"

#include <algorithm>

namespace autocompletion
{

binary_generator::binary_generator(vocabulary_type vocabulary)
: m_vocabulary { std::move(vocabulary) }
{
    // lexicographical comparison
    std::sort(std::begin(m_vocabulary), std::end(m_vocabulary));
}

auto binary_generator::autocomplete(input prefix) const -> output
{
    // lexicographical comparison
    const auto first = std::lower_bound(
        std::cbegin(m_vocabulary), std::cend(m_vocabulary), prefix);
    auto res = output();
    const auto has_prefix = [=](auto str) { return str.rfind(prefix, 0) == 0; };
    misc::copy_until(
        first, std::cend(m_vocabulary), std::back_inserter(res), has_prefix);
    return res;
}

} // namespace autocompletion