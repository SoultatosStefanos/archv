#include "weights/presenter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

struct backend_interface
{
    using dependency_type = std::string_view;
    using weight_type = int;

    virtual ~backend_interface() = default;

    virtual auto weight(dependency_type) const -> weight_type = 0;
    virtual auto update_weight(dependency_type, weight_type) -> void = 0;
    virtual auto restore() -> void = 0;
};

struct backend_mock : backend_interface
{
    backend_mock() = default;
    ~backend_mock() override = default;

    MOCK_METHOD(weight_type, weight, (dependency_type), (const, override));
    MOCK_METHOD(
        void, update_weight, (dependency_type, weight_type), (override));
    MOCK_METHOD(void, restore, (), (override));
};

struct dummy_view
{
    using dependency_type = std::string_view;
    using weight_type = int;

    using weight_accessor = std::function< weight_type(dependency_type) >;

    using dependency_slot = std::function< void(dependency_type, weight_type) >;
    using void_slot = std::function< void() >;

    auto set_weights(const weight_accessor&) -> void { }

    auto connect_to_dependency(const dependency_slot&) -> void { }
    auto connect_to_restore(const void_slot&) -> void { }
};

using presenter = weights::presenter< dummy_view, backend_interface >;

class given_a_weights_presenter : public Test
{
protected:
    void SetUp() override { pres = std::make_unique< presenter >(back, view); }

    NiceMock< backend_mock > back;
    dummy_view view;
    std::unique_ptr< presenter > pres;
};

TEST_F(given_a_weights_presenter, fetch_weight_delegates_to_backend)
{
    static constexpr auto let_dependency = "OO";
    static constexpr auto let_weight = 4;

    ON_CALL(back, weight(let_dependency)).WillByDefault(Return(let_weight));

    ASSERT_EQ(pres->fetch_weight(let_dependency), let_weight);
}

TEST_F(given_a_weights_presenter, select_weight_delegates_to_backend)
{
    static constexpr auto let_dependency = "OO";
    static constexpr auto let_weight = 4;

    EXPECT_CALL(back, update_weight(let_dependency, let_weight)).Times(1);

    pres->select_weight(let_dependency, let_weight);
}

TEST_F(given_a_weights_presenter, select_restore_delegates_to_backend)
{
    EXPECT_CALL(back, restore()).Times(1);

    pres->select_restore();
}

} // namespace