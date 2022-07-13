#include "utility/all.hpp"
#include "utility/event_system.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace utility;

class Given_an_event_bus : public testing::Test
{
public:
    void SetUp() override { pipeline = std::make_unique<event_bus>(); }

protected:
    std::unique_ptr<event_bus> pipeline;
};

struct dummy_event
{
    int i;

    constexpr auto operator==(const dummy_event&) const -> bool = default;
    constexpr auto operator!=(const dummy_event&) const -> bool = default;
};

TEST_F(Given_an_event_bus,
       Mock_is_called_n_times_after_subscribing_n_times_and_posting_event)
{
    using mock = testing::MockFunction<event_bus::subscriber<dummy_event>>;
    using nice_mock = testing::NiceMock<mock>;

    nice_mock f;
    const auto n = urandom(2, 100);
    for (auto i = 0; i < n; ++i)
        pipeline->subscribe<dummy_event>(f.AsStdFunction());
    dummy_event e{.i = urandom<int>()};

    EXPECT_CALL(f, Call(e)).Times(n);

    pipeline->post(e);
}

struct dummy2_event
{
    double i, j;

    constexpr auto operator==(const dummy2_event&) const -> bool = default;
    constexpr auto operator!=(const dummy2_event&) const -> bool = default;
};

TEST_F(Given_an_event_bus,
       Mock_is_not_called_when_posting_different_to_subscribed_to_event)
{
    using mock = testing::MockFunction<event_bus::subscriber<dummy_event>>;
    using nice_mock = testing::NiceMock<mock>;

    nice_mock f;
    pipeline->subscribe<dummy_event>(f.AsStdFunction());

    EXPECT_CALL(f, Call(testing::_)).Times(0);

    pipeline->post(dummy2_event{.i = 9, .j = 90.2});
}

} // namespace