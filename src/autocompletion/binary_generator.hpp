// Contains an autocomplete generator using binary search.
// Soultatos Stefanos 2022

#ifndef AUTOCOMPLETION_BINARY_GENERATOR_HPP
#define AUTOCOMPLETION_BINARY_GENERATOR_HPP

#include <ranges>      // for input_range, range_value_t
#include <string_view> // for string_view
#include <vector>      // for vector

namespace autocompletion
{

/***********************************************************
 * Binary generator                                        *
 ***********************************************************/

class binary_generator
{
public:
    using vocabulary_type = std::vector< std::string >;
    using input = std::string_view;
    using output = std::vector< std::string_view >;

    explicit binary_generator(vocabulary_type vocabulary);

    auto autocomplete(input prefix) const -> output;

private:
    vocabulary_type m_vocabulary;
};

} // namespace autocompletion

#endif // AUTOCOMPLETION_BINARY_GENERATOR_HPP
