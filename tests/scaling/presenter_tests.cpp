#include "scaling/presenter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

struct backend_interface
{
    using tag_type = std::string_view;
    using dims_type = int;
    using baseline_type = int;
    using enabled_type = bool;
    using ratio_type = float;

    virtual ~backend_interface() = default;

    virtual auto factor_baseline(tag_type) const -> baseline_type = 0;
    virtual auto factor_dims(tag_type) const -> dims_type = 0;
    virtual auto factor_enabled(tag_type) const -> enabled_type = 0;
    virtual auto factor_min_ratio(tag_type) const -> ratio_type = 0;
    virtual auto factor_max_ratio(tag_type) const -> ratio_type = 0;

    virtual auto update_factor_baseline(tag_type, baseline_type) -> void = 0;
    virtual auto update_factor_dims(tag_type, dims_type) -> void = 0;
    virtual auto update_factor_enabled(tag_type, enabled_type) -> void = 0;
    virtual auto update_factor_min_ratio(tag_type, ratio_type) -> void = 0;
    virtual auto update_factor_max_ratio(tag_type, ratio_type) -> void = 0;
    virtual auto restore() -> void = 0;
};

struct backend_mock : backend_interface
{
    backend_mock() = default;
    ~backend_mock() override = default;

    MOCK_METHOD(baseline_type, factor_baseline, (tag_type), (const));
    MOCK_METHOD(dims_type, factor_dims, (tag_type), (const));
    MOCK_METHOD(enabled_type, factor_enabled, (tag_type), (const));
    MOCK_METHOD(ratio_type, factor_min_ratio, (tag_type), (const));
    MOCK_METHOD(ratio_type, factor_max_ratio, (tag_type), (const));

    MOCK_METHOD(void, update_factor_baseline, (tag_type, baseline_type));
    MOCK_METHOD(void, update_factor_dims, (tag_type, dims_type));
    MOCK_METHOD(void, update_factor_enabled, (tag_type, enabled_type));
    MOCK_METHOD(void, update_factor_min_ratio, (tag_type, ratio_type));
    MOCK_METHOD(void, update_factor_max_ratio, (tag_type, ratio_type));
    MOCK_METHOD(void, restore, (), (override));
};

struct dummy_view
{
    using tag_type = std::string_view;
    using dims_type = int;
    using baseline_type = int;
    using enabled_type = bool;
    using ratio_type = float;

    using dims_accessor = std::function< dims_type(tag_type) >;
    using baseline_accessor = std::function< baseline_type(tag_type) >;
    using enabled_accessor = std::function< enabled_type(tag_type) >;
    using ratio_accessor = std::function< ratio_type(tag_type) >;

    using dims_slot = std::function< void(tag_type, dims_type) >;
    using baseline_slot = std::function< void(tag_type, baseline_type) >;
    using enabled_slot = std::function< void(tag_type, enabled_type) >;
    using ratio_slot = std::function< void(tag_type, ratio_type) >;
    using restore_slot = std::function< void() >;

    auto set_dims(const dims_accessor&) -> void { }
    auto set_baseline(const baseline_accessor&) -> void { }
    auto set_enabled(const enabled_accessor&) -> void { }
    auto set_min_ratio(const ratio_accessor&) -> void { }
    auto set_max_ratio(const ratio_accessor&) -> void { }

    auto connect_to_dims(const dims_slot&) -> void { }
    auto connect_to_baseline(const baseline_slot&) -> void { }
    auto connect_to_enabled(const enabled_slot&) -> void { }
    auto connect_to_min_ratio(const ratio_slot&) -> void { }
    auto connect_to_max_ratio(const ratio_slot&) -> void { }
    auto connect_to_restore(const restore_slot&) -> void { }
};

using presenter = scaling::presenter< dummy_view, backend_interface >;

class given_a_scaling_presenter : public Test
{
protected:
    void SetUp() override { pres = std::make_unique< presenter >(back, view); }

    NiceMock< backend_mock > back;
    dummy_view view;
    std::unique_ptr< presenter > pres;
};

TEST_F(given_a_scaling_presenter, fetch_factor_baseline_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_baseline = 90;

    ON_CALL(back, factor_baseline(let_tag)).WillByDefault(Return(let_baseline));

    ASSERT_EQ(pres->fetch_factor_baseline(let_tag), let_baseline);
}

TEST_F(given_a_scaling_presenter, fetch_factor_dims_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_dims = 3;

    ON_CALL(back, factor_dims(let_tag)).WillByDefault(Return(let_dims));

    ASSERT_EQ(pres->fetch_factor_dims(let_tag), let_dims);
}

TEST_F(given_a_scaling_presenter, fetch_factor_enabled_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_enabled = false;

    ON_CALL(back, factor_enabled(let_tag)).WillByDefault(Return(let_enabled));

    ASSERT_EQ(pres->fetch_factor_enabled(let_tag), let_enabled);
}

TEST_F(given_a_scaling_presenter, fetch_factor_min_ratio_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_ratio = 23;

    ON_CALL(back, factor_min_ratio(let_tag)).WillByDefault(Return(let_ratio));

    ASSERT_EQ(pres->fetch_factor_min_ratio(let_tag), let_ratio);
}

TEST_F(given_a_scaling_presenter, fetch_factor_max_ratio_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_ratio = 23;

    ON_CALL(back, factor_max_ratio(let_tag)).WillByDefault(Return(let_ratio));

    ASSERT_EQ(pres->fetch_factor_max_ratio(let_tag), let_ratio);
}

TEST_F(given_a_scaling_presenter, select_factor_baseline_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_baseline = 23;

    EXPECT_CALL(back, update_factor_baseline(let_tag, let_baseline)).Times(1);

    pres->select_factor_baseline(let_tag, let_baseline);
}

TEST_F(given_a_scaling_presenter, select_factor_dims_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_dims = 23;

    EXPECT_CALL(back, update_factor_dims(let_tag, let_dims)).Times(1);

    pres->select_factor_dims(let_tag, let_dims);
}

TEST_F(given_a_scaling_presenter, select_factor_enabled_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_enabled = false;

    EXPECT_CALL(back, update_factor_enabled(let_tag, let_enabled)).Times(1);

    pres->select_factor_enabled(let_tag, let_enabled);
}

TEST_F(given_a_scaling_presenter, select_factor_min_ratio_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_ratio = 0;

    EXPECT_CALL(back, update_factor_min_ratio(let_tag, let_ratio)).Times(1);

    pres->select_factor_min_ratio(let_tag, let_ratio);
}

TEST_F(given_a_scaling_presenter, select_factor_max_ratio_delegates_to_backend)
{
    static constexpr auto let_tag = "ASA";
    static constexpr auto let_ratio = 0;

    EXPECT_CALL(back, update_factor_max_ratio(let_tag, let_ratio)).Times(1);

    pres->select_factor_max_ratio(let_tag, let_ratio);
}

TEST_F(given_a_scaling_presenter, select_restore_delegates_to_backend)
{
    EXPECT_CALL(back, restore()).Times(1);

    pres->select_restore();
}

} // namespace