#include "ui/ui_component.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

struct dummy_config
{
    int i { 0 };

    constexpr auto operator==(const dummy_config&) const -> bool = default;
    constexpr auto operator!=(const dummy_config&) const -> bool = default;
};

struct dummy_config_api
{
    dummy_config cfg;

    auto config_data() const -> const dummy_config& { return cfg; }
    auto config_data() -> dummy_config& { return cfg; }
};

dummy_config drawn_data;

struct dummy_ui_component
{
    using config_data_type = dummy_config;
    using config_api_type = dummy_config_api;

    dummy_config cfg, default_cfg;
    dummy_config_api api;

    auto default_data() const -> const dummy_config& { return default_cfg; }

    auto config_data() const -> const dummy_config& { return cfg; }
    auto config_data() -> dummy_config& { return cfg; }

    auto config_api() const -> const dummy_config_api& { return api; }
    auto config_api() -> dummy_config_api& { return api; }

    auto draw(const dummy_config& cfg) const -> void { drawn_data = cfg; }
};

class given_a_dummy_ui_component : public Test
{
protected:
    void SetUp() override
    {
        ui_comp = std::make_unique< dummy_ui_component >();
    }

    std::unique_ptr< dummy_ui_component > ui_comp;
};

TEST_F(given_a_dummy_ui_component, begin_preview_draws_intermediate_data)
{
    ui_comp->config_api().config_data().i = 30;

    ui::begin_preview(*ui_comp);

    ASSERT_EQ(drawn_data, ui_comp->config_api().config_data());
    ASSERT_TRUE(drawn_data.i == 30);
}

TEST_F(given_a_dummy_ui_component, end_preview_draws_current_data)
{
    ui_comp->config_api().config_data().i = 30;

    ui::begin_preview(*ui_comp);

    EXPECT_EQ(drawn_data, ui_comp->config_api().config_data());
    EXPECT_TRUE(drawn_data.i == 30);

    ui::end_preview(*ui_comp);

    ASSERT_EQ(drawn_data, ui_comp->config_data());
    ASSERT_TRUE(drawn_data.i == 0);
}

TEST_F(
    given_a_dummy_ui_component,
    apply_config_draws_and_sets_intermediate_data)
{
    ui_comp->config_api().config_data().i = 30;

    ui::apply_configs(*ui_comp);

    ASSERT_EQ(drawn_data, ui_comp->config_data());
    ASSERT_EQ(ui_comp->config_data(), ui_comp->config_api().config_data());
    ASSERT_TRUE(drawn_data.i == 30);
}

TEST_F(
    given_a_dummy_ui_component,
    restore_defaults_draws_and_sets_intermediate_data)
{
    ui_comp->config_api().config_data().i = 30;

    ui::apply_configs(*ui_comp);

    EXPECT_EQ(drawn_data, ui_comp->config_data());
    EXPECT_EQ(ui_comp->config_data(), ui_comp->config_api().config_data());
    EXPECT_TRUE(drawn_data.i == 30);

    ui::restore_defaults(*ui_comp);

    ASSERT_EQ(drawn_data, ui_comp->default_cfg);
    ASSERT_EQ(ui_comp->config_data(), ui_comp->default_cfg);
    ASSERT_TRUE(drawn_data.i == 0);
}

TEST_F(given_a_dummy_ui_component, import_configs_applies_new_configs)
{
    dummy_config cfg { .i = 20 };
    ui::import_configs(*ui_comp, cfg);

    EXPECT_EQ(drawn_data, cfg);
    EXPECT_EQ(ui_comp->config_data(), cfg);
    EXPECT_EQ(ui_comp->config_api().config_data(), cfg);
}

TEST_F(given_a_dummy_ui_component, export_configs_simply_returns_a_config_copy)
{
    dummy_config cfg { .i = 20 };
    ui::import_configs(*ui_comp, cfg);
    ASSERT_EQ(ui_comp->config_data(), cfg);

    EXPECT_EQ(ui::export_configs(*ui_comp), cfg);
}

} // namespace