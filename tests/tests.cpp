#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

static void init_logging()
{
    namespace logging = boost::log;

    logging::core::get()->set_logging_enabled(false);
}

auto main(int argc, char* argv[]) -> int
{
    init_logging();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}