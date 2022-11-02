#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

static void init_logging(bool enabled)
{
    namespace logging = boost::log;

    logging::core::get()->set_logging_enabled(enabled);
}

auto main(int argc, char* argv[]) -> int
{
    bool log = false;

    if (argc == 2 && std::string_view(argv[1]) == "log")
    {
        log = true;
    }

    init_logging(log);

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}