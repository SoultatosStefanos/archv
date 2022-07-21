// Contains the base error handling class of the configuration package.
// Soultatos Stefanos 2022

#ifndef CONFIG_ERROR_HPP
#define CONFIG_ERROR_HPP

#include <boost/exception/all.hpp>
#include <stdexcept>

namespace config
{

// To be inherited from at config/
struct error : virtual std::exception, virtual boost::exception
{
};

} // namespace config

#endif // CONFIG_ERROR_HPP
