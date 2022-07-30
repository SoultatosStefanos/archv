#include "app_.hpp"

#include <boost/exception/all.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <cstdlib>

static void init_logging()
{
    namespace logging = boost::log;

#ifdef NDEBUG
    logging::core::get()->set_filter(logging::trivial::severity >=
                                     logging::trivial::info);
#endif
}

auto main(int argc, char const* argv[]) -> int
{
    try
    {
        init_logging();

        app archv;
        archv.initApp();
        archv.getRoot()->startRendering();
        archv.closeApp();

        return EXIT_SUCCESS;
    }
    catch (const boost::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "error:\n"
                                 << boost::diagnostic_information(e);

        return EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "error: " << e.what();

        return EXIT_FAILURE;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(fatal) << "unknown error";

        return EXIT_FAILURE;
    }
}
