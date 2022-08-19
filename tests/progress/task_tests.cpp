#include "progress/task.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace progress;
using namespace testing;

namespace
{

class mock_task : public task
{
public:
    mock_task() = default;
    ~mock_task() = default;

    MOCK_METHOD(units, total_units, (), (const, override));
    MOCK_METHOD(units, units_done, (), (const, override));

    MOCK_METHOD(void, stop, (), (override));
    MOCK_METHOD(void, suspend, (), (override));
    MOCK_METHOD(void, resume, (), (override));

    MOCK_METHOD(void, work, (units), (override));
};

class given_a_task : public Test
{
public:
    given_a_task() : mock { std::make_unique< mock_type >() } { }

protected:
    using mock_type = NiceMock< mock_task >;

    std::unique_ptr< mock_type > mock;
};

TEST_F(given_a_task, percentage_done_gives_right_result_in_units)
{
    EXPECT_CALL(*mock, total_units).Times(1).WillOnce(Return(10));
    EXPECT_CALL(*mock, units_done).Times(1).WillOnce(Return(5));

    ASSERT_EQ(50, percentage_done(*mock));
}

} // namespace