#include "layout/presenter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

struct backend_interface
{
    using layout_id_type = std::string_view;
    using topology_id_type = std::string_view;
    using scale_type = int;

    virtual ~backend_interface() = default;

    virtual auto layout_id() const -> layout_id_type = 0;
    virtual auto topology_id() const -> topology_id_type = 0;
    virtual auto scale() const -> scale_type = 0;
    virtual auto update_layout(layout_id_type) -> void = 0;
    virtual auto update_topology(topology_id_type) -> void = 0;
    virtual auto update_scale(scale_type) -> void = 0;
    virtual auto restore() -> void = 0;
};

struct backend_mock : backend_interface
{
    backend_mock() = default;
    ~backend_mock() override = default;

    MOCK_METHOD(layout_id_type, layout_id, (), (const, override));
    MOCK_METHOD(topology_id_type, topology_id, (), (const, override));
    MOCK_METHOD(scale_type, scale, (), (const, override));
    MOCK_METHOD(void, update_layout, (layout_id_type), (override));
    MOCK_METHOD(void, update_topology, (topology_id_type), (override));
    MOCK_METHOD(void, update_scale, (scale_type), (override));
    MOCK_METHOD(void, restore, (), (override));
};

struct dummy_view
{
    using layout_type = std::string_view;
    using topology_type = std::string_view;
    using scale_type = int;

    using layout_accessor = std::function< layout_type() >;
    using topology_accessor = std::function< topology_type() >;
    using scale_accessor = std::function< scale_type() >;

    using layout_slot = std::function< void(layout_type) >;
    using topology_slot = std::function< void(topology_type) >;
    using scale_slot = std::function< void(scale_type) >;
    using void_slot = std::function< void() >;

    auto set_layout(const layout_accessor&) -> void { }
    auto set_topology(const topology_accessor&) -> void { }
    auto set_scale(const scale_accessor&) -> void { }

    auto connect_to_layout(const layout_slot&) -> void { }
    auto connect_to_topology(const topology_slot&) -> void { }
    auto connect_to_scale(const scale_slot&) -> void { }
    auto connect_to_restore(const void_slot&) -> void { }
};

using presenter = layout::presenter< dummy_view, backend_interface >;

class given_a_layout_presenter : public Test
{
protected:
    void SetUp() override { pres = std::make_unique< presenter >(back, view); }

    NiceMock< backend_mock > back;
    dummy_view view;
    std::unique_ptr< presenter > pres;
};

TEST_F(given_a_layout_presenter, fetch_layout_delegates_to_backend)
{
    constexpr auto let_layout_id = "Bob";

    ON_CALL(back, layout_id).WillByDefault(Return(let_layout_id));

    ASSERT_EQ(pres->fetch_layout(), let_layout_id);
}

TEST_F(given_a_layout_presenter, fetch_topology_delegates_to_backend)
{
    constexpr auto let_topology_id = "Alice";

    ON_CALL(back, topology_id).WillByDefault(Return(let_topology_id));

    ASSERT_EQ(pres->fetch_topology(), let_topology_id);
}

TEST_F(given_a_layout_presenter, fetch_scale_delegates_to_backend)
{
    constexpr auto let_scale = 34;

    ON_CALL(back, scale).WillByDefault(Return(let_scale));

    ASSERT_EQ(pres->fetch_scale(), let_scale);
}

TEST_F(given_a_layout_presenter, select_layout_delegates_to_backend)
{
    constexpr auto let_layout_id = "Bob";

    EXPECT_CALL(back, update_layout(let_layout_id)).Times(1);

    pres->select_layout(let_layout_id);
}

TEST_F(given_a_layout_presenter, select_topology_delegates_to_backend)
{
    constexpr auto let_topology_id = "Bob";

    EXPECT_CALL(back, update_topology(let_topology_id)).Times(1);

    pres->select_topology(let_topology_id);
}

TEST_F(given_a_layout_presenter, select_scale_delegates_to_backend)
{
    constexpr auto let_scale = 234;

    EXPECT_CALL(back, update_scale(let_scale)).Times(1);

    pres->select_scale(let_scale);
}

TEST_F(given_a_layout_presenter, select_restore_delegates_to_backend)
{
    EXPECT_CALL(back, restore()).Times(1);

    pres->select_restore();
}

} // namespace