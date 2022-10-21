#include "application/gui_presenter.hpp"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

struct config
{
    using name_type = std::string;
    using rounding_type = int;
    using bordered_type = bool;

    name_type color_theme;
    rounding_type frame_rounding;
    bordered_type window_bordered;
    bordered_type frame_bordered;
    bordered_type popup_bordered;

    auto operator==(const config&) const -> bool = default;
    auto operator!=(const config&) const -> bool = default;
};

constexpr auto color_theme = "aa";
constexpr auto frame_rounding = 0;
constexpr auto window_bordered = false;
constexpr auto frame_bordered = true;
constexpr auto popup_bordered = true;

inline auto make_config()
{
    return config { .color_theme = color_theme,
                    .frame_rounding = frame_rounding,
                    .window_bordered = window_bordered,
                    .frame_bordered = frame_bordered,
                    .popup_bordered = popup_bordered };
}

struct config_api_interface
{
    using config_data_type = config;
    using name_type = config_data_type::name_type;
    using rounding_type = config_data_type::rounding_type;
    using bordered_type = config_data_type::bordered_type;

    virtual ~config_api_interface() = default;

    virtual auto config_data() const -> const config_data_type& = 0;
    virtual auto config_data() -> config_data_type& = 0;

    virtual auto set_color_theme(name_type) -> void = 0;
    virtual auto set_frame_rounding(rounding_type) -> void = 0;
    virtual auto set_window_bordered(bordered_type) -> void = 0;
    virtual auto set_frame_bordered(bordered_type) -> void = 0;
    virtual auto set_popup_bordered(bordered_type) -> void = 0;
};

struct mock_config_api : config_api_interface
{
    config_data_type cfg = make_config();

    ~mock_config_api() override = default;

    auto config_data() const -> const config_data_type& { return cfg; }
    auto config_data() -> config_data_type& { return cfg; }

    MOCK_METHOD(void, set_color_theme, (name_type), (override));
    MOCK_METHOD(void, set_frame_rounding, (rounding_type), (override));
    MOCK_METHOD(void, set_window_bordered, (bordered_type), (override));
    MOCK_METHOD(void, set_frame_bordered, (bordered_type), (override));
    MOCK_METHOD(void, set_popup_bordered, (bordered_type), (override));
};

struct gui_interface
{
    using config_data_type = config;
    using config_api_type = config_api_interface;

    virtual ~gui_interface() = default;

    virtual auto default_data() const -> const config_data_type& = 0;
    virtual auto config_data() const -> const config_data_type& = 0;
    virtual auto config_data() -> config_data_type& = 0;
    virtual auto config_api() const -> const config_api_type& = 0;
    virtual auto config_api() -> config_api_type& = 0;

    virtual auto draw(const config_data_type&) -> void = 0;
};

struct mock_gui : gui_interface
{
    ~mock_gui() override = default;

    config_data_type cfg = make_config();
    NiceMock< mock_config_api > api;

    auto default_data() const -> const config_data_type& { return cfg; }
    auto config_data() const -> const config_data_type& { return cfg; }
    auto config_data() -> config_data_type& { return cfg; }
    auto config_api() const -> const config_api_type& { return api; }
    auto config_api() -> config_api_type& { return api; }

    MOCK_METHOD(void, draw, (const config_data_type&), (override));
};

struct configurator_interface
{
    using name_type = std::string_view;
    using rounding_type = int;
    using bordered_type = bool;

    using name_slot = std::function< void(name_type) >;
    using rounding_slot = std::function< void(rounding_type) >;
    using bordered_slot = std::function< void(bordered_type) >;
    using void_slot = std::function< void() >;

    using connection = void;

    virtual ~configurator_interface() = default;

    virtual auto set_color_theme(name_type) -> void = 0;
    virtual auto set_frame_rounding(rounding_type) -> void = 0;
    virtual auto set_window_bordered(bordered_type) -> void = 0;
    virtual auto set_frame_bordered(bordered_type) -> void = 0;
    virtual auto set_popup_bordered(bordered_type) -> void = 0;

    virtual auto connect_to_color_theme(name_slot) -> void = 0;
    virtual auto connect_to_frame_rounding(rounding_slot) -> void = 0;
    virtual auto connect_to_window_bordered(bordered_slot) -> void = 0;
    virtual auto connect_to_frame_bordered(bordered_slot) -> void = 0;
    virtual auto connect_to_popup_bordered(bordered_slot) -> void = 0;
    virtual auto connect_to_apply(void_slot) -> void = 0;
    virtual auto connect_to_preview(void_slot) -> void = 0;
    virtual auto connect_to_cancel(void_slot) -> void = 0;
    virtual auto connect_to_restore(void_slot) -> void = 0;
};

struct mock_configurator : configurator_interface
{
    ~mock_configurator() override = default;

    MOCK_METHOD(void, set_color_theme, (name_type), (override));
    MOCK_METHOD(void, set_frame_rounding, (rounding_type), (override));
    MOCK_METHOD(void, set_window_bordered, (bordered_type), (override));
    MOCK_METHOD(void, set_frame_bordered, (bordered_type), (override));
    MOCK_METHOD(void, set_popup_bordered, (bordered_type), (override));

    MOCK_METHOD(void, connect_to_color_theme, (name_slot), (override));
    MOCK_METHOD(void, connect_to_frame_rounding, (rounding_slot), (override));
    MOCK_METHOD(void, connect_to_window_bordered, (bordered_slot), (override));
    MOCK_METHOD(void, connect_to_frame_bordered, (bordered_slot), (override));
    MOCK_METHOD(void, connect_to_popup_bordered, (bordered_slot), (override));
    MOCK_METHOD(void, connect_to_apply, (void_slot), (override));
    MOCK_METHOD(void, connect_to_preview, (void_slot), (override));
    MOCK_METHOD(void, connect_to_cancel, (void_slot), (override));
    MOCK_METHOD(void, connect_to_restore, (void_slot), (override));
};

using gui_presenter
    = application::gui_presenter< gui_interface, configurator_interface >;

class given_a_gui_presenter : public Test
{
protected:
    void SetUp() override
    {
        pres = std::make_unique< gui_presenter >(root, configurator);
    }

    std::unique_ptr< gui_presenter > pres;
    NiceMock< mock_gui > root;
    NiceMock< mock_configurator > configurator;
};

TEST_F(
    given_a_gui_presenter,
    fetch_color_theme_sets_the_configurator_theme_from_the_gui_config_data)
{
    EXPECT_CALL(configurator, set_color_theme(color_theme)).Times(1);

    pres->fetch_color_theme();
}

TEST_F(
    given_a_gui_presenter,
    fetch_frame_rounding_sets_the_configurator_rounding_from_the_gui_config_data)
{
    EXPECT_CALL(configurator, set_frame_rounding(frame_rounding)).Times(1);

    pres->fetch_frame_rounding();
}

TEST_F(
    given_a_gui_presenter,
    fetch_frame_bordered_sets_the_configurator_bordered_from_the_gui_config_data)
{
    EXPECT_CALL(configurator, set_frame_bordered(frame_bordered)).Times(1);

    pres->fetch_frame_bordered();
}

TEST_F(
    given_a_gui_presenter,
    fetch_window_bordered_sets_the_configurator_bordered_from_the_gui_config_data)
{
    EXPECT_CALL(configurator, set_window_bordered(window_bordered)).Times(1);

    pres->fetch_window_bordered();
}

TEST_F(
    given_a_gui_presenter,
    fetch_popup_bordered_sets_the_configurator_bordered_from_the_gui_config_data)
{
    EXPECT_CALL(configurator, set_popup_bordered(popup_bordered)).Times(1);

    pres->fetch_popup_bordered();
}

TEST_F(
    given_a_gui_presenter,
    select_color_theme_sets_the_gui_theme_through_its_api)
{
    EXPECT_CALL(root.api, set_color_theme("a")).Times(1);

    pres->select_color_theme("a");
}

TEST_F(
    given_a_gui_presenter,
    select_frame_rounding_sets_the_gui_rounding_through_its_api)
{
    EXPECT_CALL(root.api, set_frame_rounding(3)).Times(1);

    pres->select_frame_rounding(3);
}

TEST_F(
    given_a_gui_presenter,
    select_window_bordered_sets_the_gui_bordered_through_its_api)
{
    EXPECT_CALL(root.api, set_window_bordered(true)).Times(1);

    pres->select_window_bordered(true);
}

TEST_F(
    given_a_gui_presenter,
    select_frame_bordered_sets_the_gui_bordered_through_its_api)
{
    EXPECT_CALL(root.api, set_frame_bordered(true)).Times(1);

    pres->select_frame_bordered(true);
}

TEST_F(
    given_a_gui_presenter,
    select_popup_bordered_sets_the_gui_bordered_through_its_api)
{
    EXPECT_CALL(root.api, set_popup_bordered(true)).Times(1);

    pres->select_popup_bordered(true);
}

TEST_F(given_a_gui_presenter, select_apply_draws_the_gui_config_api_data)
{
    EXPECT_CALL(root, draw(root.api.config_data())).Times(1);

    pres->select_apply();
}

TEST_F(given_a_gui_presenter, select_preview_draws_the_gui_config_api_data)
{
    EXPECT_CALL(root, draw(root.api.config_data())).Times(1);

    pres->select_preview();
}

TEST_F(given_a_gui_presenter, select_cancel_draws_the_gui_config_data)
{
    EXPECT_CALL(root, draw(root.config_data())).Times(1);

    pres->select_cancel();
}

TEST_F(
    given_a_gui_presenter,
    select_restore_draws_the_default_gui_data_and_fetches_the_configurator_values)
{
    EXPECT_CALL(root, draw(root.default_data())).Times(1);
    EXPECT_CALL(configurator, set_color_theme(color_theme)).Times(1);
    EXPECT_CALL(configurator, set_frame_rounding(frame_rounding)).Times(1);
    EXPECT_CALL(configurator, set_frame_bordered(frame_bordered)).Times(1);
    EXPECT_CALL(configurator, set_window_bordered(window_bordered)).Times(1);
    EXPECT_CALL(configurator, set_popup_bordered(popup_bordered)).Times(1);

    pres->select_restore();
}

using presenter = application::gui_presenter<>;

} // namespace