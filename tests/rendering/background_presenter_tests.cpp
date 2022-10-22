#include "rendering/background_presenter.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;

namespace
{

/***********************************************************
 * Mocks                                                   *
 ***********************************************************/

struct config_data_t
{
    using name_type = Ogre::String;
    using distance_type = Ogre::Real;
    using rgba_type = Ogre::ColourValue;

    name_type skybox_material;
    distance_type skybox_distance;
    rgba_type ambient_color;
    rgba_type diffuse_color;
    rgba_type specular_color;
    distance_type cam_near_clip_distance;
    distance_type cam_far_clip_distance;

    auto operator==(const config_data_t&) const -> bool = default;
    auto operator!=(const config_data_t&) const -> bool = default;
};

constexpr auto skybox_material = "material";
constexpr auto skybox_distance = 2;
const auto ambient_color = config_data_t::rgba_type(1, 1, 1, 0);
const auto diffuse_color = config_data_t::rgba_type(13, 11, 12, 0);
const auto specular_color = config_data_t::rgba_type(1, 31, 10, 0);
constexpr auto cam_near_clip_distance = 0;
constexpr auto cam_far_clip_distance = 54;

inline auto make_config_data() -> config_data_t
{
    return { .skybox_material = skybox_material,
             .skybox_distance = skybox_distance,
             .ambient_color = ambient_color,
             .diffuse_color = diffuse_color,
             .specular_color = specular_color,
             .cam_near_clip_distance = cam_near_clip_distance,
             .cam_far_clip_distance = cam_far_clip_distance };
}

struct config_api_interface
{
    using config_data_type = config_data_t;

    virtual ~config_api_interface() = default;

    virtual auto config_data() const -> const config_data_type& = 0;
    virtual auto config_data() -> config_data_type& = 0;
    virtual auto set_skybox_material(Ogre::String) -> void = 0;
    virtual auto set_skybox_distance(Ogre::Real) -> void = 0;
    virtual auto set_ambient_color(Ogre::ColourValue) -> void = 0;
    virtual auto set_diffuse_color(Ogre::ColourValue) -> void = 0;
    virtual auto set_specular_color(Ogre::ColourValue) -> void = 0;
    virtual auto set_cam_far_clip_distance(Ogre::Real) -> void = 0;
    virtual auto set_cam_near_clip_distance(Ogre::Real) -> void = 0;
};

struct mock_config_api : public config_api_interface
{
    mock_config_api() = default;
    virtual ~mock_config_api() override = default;

    config_data_type config = make_config_data();

    auto config_data() const -> const config_data_type& { return config; }
    auto config_data() -> config_data_type& { return config; }
    MOCK_METHOD(void, set_skybox_material, (Ogre::String), (override));
    MOCK_METHOD(void, set_skybox_distance, (Ogre::Real), (override));
    MOCK_METHOD(void, set_ambient_color, (Ogre::ColourValue), (override));
    MOCK_METHOD(void, set_diffuse_color, (Ogre::ColourValue), (override));
    MOCK_METHOD(void, set_specular_color, (Ogre::ColourValue), (override));
    MOCK_METHOD(void, set_cam_far_clip_distance, (Ogre::Real), (override));
    MOCK_METHOD(void, set_cam_near_clip_distance, (Ogre::Real), (override));
};

struct renderer_interface
{
    using config_data_type = config_data_t;
    using config_api_type = config_api_interface;

    renderer_interface() = default;
    virtual ~renderer_interface() = default;

    virtual auto default_data() const -> const config_data_type& = 0;
    virtual auto config_data() const -> const config_data_type& = 0;
    virtual auto config_data() -> config_data_type& = 0;
    virtual auto config_api() const -> const config_api_type& = 0;
    virtual auto config_api() -> config_api_type& = 0;
    virtual auto draw(const config_data_type&) -> void = 0;
};

struct mock_renderer : public renderer_interface
{
    mock_renderer() = default;
    virtual ~mock_renderer() override = default;

    NiceMock< mock_config_api > api;
    config_data_type config = make_config_data();

    auto default_data() const -> const config_data_type& { return config; }
    auto config_data() const -> const config_data_type& { return config; }
    auto config_data() -> config_data_type& { return config; }
    auto config_api() const -> const config_api_type& { return api; }
    auto config_api() -> config_api_type& { return api; }

    MOCK_METHOD(void, draw, (const config_data_type&), (override));
};

struct view_interface
{
    using name_type = std::string_view;
    using distance_type = float;
    using rgba_type = std::array< float, 4 >;

    using name_slot = std::function< void(name_type) >;
    using distance_slot = std::function< void(distance_type) >;
    using rgba_slot = std::function< void(const rgba_type&) >;
    using void_slot = std::function< void() >;

    view_interface() = default;
    virtual ~view_interface() = default;

    virtual auto set_skybox_material(name_type) -> void = 0;
    virtual auto set_skybox_distance(distance_type) -> void = 0;
    virtual auto set_ambient_color(rgba_type) -> void = 0;
    virtual auto set_diffuse_color(rgba_type) -> void = 0;
    virtual auto set_specular_color(rgba_type) -> void = 0;
    virtual auto set_cam_far_clip_distance(distance_type) -> void = 0;
    virtual auto set_cam_near_clip_distance(distance_type) -> void = 0;

    virtual auto connect_to_skybox_material(name_slot) -> void = 0;
    virtual auto connect_to_skybox_distance(distance_slot) -> void = 0;
    virtual auto connect_to_ambient_color(rgba_slot) -> void = 0;
    virtual auto connect_to_diffuse_color(rgba_slot) -> void = 0;
    virtual auto connect_to_specular_color(rgba_slot) -> void = 0;
    virtual auto connect_to_cam_far_clip_distance(distance_slot) -> void = 0;
    virtual auto connect_to_cam_near_clip_distance(distance_slot) -> void = 0;
    virtual auto connect_to_apply(void_slot) -> void = 0;
    virtual auto connect_to_preview(void_slot) -> void = 0;
    virtual auto connect_to_cancel(void_slot) -> void = 0;
    virtual auto connect_to_restore(void_slot) -> void = 0;
};

struct view_mock : public view_interface
{
    view_mock() = default;
    virtual ~view_mock() override = default;

    MOCK_METHOD(void, set_skybox_material, (name_type), (override));
    MOCK_METHOD(void, set_skybox_distance, (distance_type), (override));
    MOCK_METHOD(void, set_ambient_color, (rgba_type), (override));
    MOCK_METHOD(void, set_diffuse_color, (rgba_type), (override));
    MOCK_METHOD(void, set_specular_color, (rgba_type), (override));
    MOCK_METHOD(void, set_cam_far_clip_distance, (distance_type), (override));
    MOCK_METHOD(void, set_cam_near_clip_distance, (distance_type), (override));

    MOCK_METHOD(void, connect_to_skybox_material, (name_slot));
    MOCK_METHOD(void, connect_to_skybox_distance, (distance_slot));
    MOCK_METHOD(void, connect_to_ambient_color, (rgba_slot));
    MOCK_METHOD(void, connect_to_diffuse_color, (rgba_slot));
    MOCK_METHOD(void, connect_to_specular_color, (rgba_slot));
    MOCK_METHOD(void, connect_to_cam_far_clip_distance, (distance_slot));
    MOCK_METHOD(void, connect_to_cam_near_clip_distance, (distance_slot));
    MOCK_METHOD(void, connect_to_apply, (void_slot));
    MOCK_METHOD(void, connect_to_preview, (void_slot));
    MOCK_METHOD(void, connect_to_cancel, (void_slot));
    MOCK_METHOD(void, connect_to_restore, (void_slot));
};

using background_presenter
    = rendering::background_presenter< view_interface, renderer_interface >;

/***********************************************************
 * Tests                                                   *
 ***********************************************************/

class given_a_background_presenter : public Test
{
protected:
    void SetUp() override
    {
        pres = std::make_unique< background_presenter >(renderer, view);
    }

    std::unique_ptr< background_presenter > pres;
    NiceMock< mock_renderer > renderer;
    NiceMock< view_mock > view;
};

TEST_F(
    given_a_background_presenter,
    fetch_skybox_material_sets_the_view_material_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_skybox_material(skybox_material)).Times(1);

    pres->fetch_skybox_material();
}

TEST_F(
    given_a_background_presenter,
    fetch_skybox_distance_sets_the_view_distance_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_skybox_distance(skybox_distance)).Times(1);

    pres->fetch_skybox_distance();
}

TEST_F(
    given_a_background_presenter,
    fetch_ambient_color_sets_the_view_ambient_color_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_ambient_color({ { 1, 1, 1, 0 } })).Times(1);

    pres->fetch_ambient_color();
}

TEST_F(
    given_a_background_presenter,
    fetch_diffuse_color_sets_the_view_diffuse_color_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_diffuse_color({ { 13, 11, 12, 0 } })).Times(1);

    pres->fetch_diffuse_color();
}

TEST_F(
    given_a_background_presenter,
    fetch_specular_color_sets_the_view_specular_color_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_specular_color({ { 1, 31, 10, 0 } })).Times(1);

    pres->fetch_specular_color();
}

TEST_F(
    given_a_background_presenter,
    fetch_cam_far_clip_distance_sets_the_view_distance_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_cam_far_clip_distance(cam_far_clip_distance))
        .Times(1);

    pres->fetch_cam_far_clip_distance();
}

TEST_F(
    given_a_background_presenter,
    fetch_cam_near_clip_distance_sets_the_view_distance_from_the_renderer_config_data)
{
    EXPECT_CALL(view, set_cam_near_clip_distance(cam_near_clip_distance))
        .Times(1);

    pres->fetch_cam_near_clip_distance();
}

TEST_F(
    given_a_background_presenter,
    select_skybox_material_sets_the_renderer_material_through_its_config_api)
{
    EXPECT_CALL(renderer.api, set_skybox_material("bob")).Times(1);

    pres->select_skybox_material("bob");
}

TEST_F(
    given_a_background_presenter,
    select_skybox_distance_sets_the_renderer_distance_through_its_config_api)
{
    EXPECT_CALL(renderer.api, set_skybox_distance(90)).Times(1);

    pres->select_skybox_distance(90);
}

TEST_F(
    given_a_background_presenter,
    select_ambient_color_sets_the_renderer_color_through_its_config_api)
{
    EXPECT_CALL(
        renderer.api,
        set_ambient_color(config_data_t::rgba_type { 1, 1, 1, 1 }))
        .Times(1);

    pres->select_ambient_color({ { 1, 1, 1, 1 } });
}

TEST_F(
    given_a_background_presenter,
    select_diffuse_color_sets_the_renderer_color_through_its_config_api)
{
    EXPECT_CALL(
        renderer.api,
        set_diffuse_color(config_data_t::rgba_type { 1, 2, 3, 4 }))
        .Times(1);

    pres->select_diffuse_color({ { 1, 2, 3, 4 } });
}

TEST_F(
    given_a_background_presenter,
    select_specular_color_sets_the_renderer_color_through_its_config_api)
{
    EXPECT_CALL(
        renderer.api,
        set_specular_color(config_data_t::rgba_type { 1, 2, 3, 4 }))
        .Times(1);

    pres->select_specular_color({ { 1, 2, 3, 4 } });
}

TEST_F(
    given_a_background_presenter,
    select_cam_far_clip_distance_sets_the_renderer_dist_through_its_config_api)
{
    EXPECT_CALL(renderer.api, set_cam_far_clip_distance(1)).Times(1);

    pres->select_cam_far_clip_distance(1);
}

TEST_F(
    given_a_background_presenter,
    select_cam_near_clip_distance_sets_the_renderer_dist_through_its_config_api)
{
    EXPECT_CALL(renderer.api, set_cam_near_clip_distance(1)).Times(1);

    pres->select_cam_near_clip_distance(1);
}

TEST_F(
    given_a_background_presenter,
    select_apply_draws_the_renderer_config_data_through_its_api)
{
    EXPECT_CALL(renderer, draw(renderer.api.config)).Times(1);

    pres->select_apply();
}

TEST_F(
    given_a_background_presenter,
    select_preview_draws_the_renderer_config_api_data_through_its_api)
{
    EXPECT_CALL(renderer, draw(renderer.api.config)).Times(1);

    pres->select_preview();
}

TEST_F(
    given_a_background_presenter,
    select_cancel_draws_the_renderer_config_data_through_its_api)
{
    EXPECT_CALL(renderer, draw(renderer.config)).Times(1);

    pres->select_cancel();
}

TEST_F(
    given_a_background_presenter,
    select_restore_draws_the_renderer_default_data_and_fetches_all)
{
    EXPECT_CALL(renderer, draw(renderer.default_data())).Times(1);

    EXPECT_CALL(view, set_skybox_material(skybox_material)).Times(1);
    EXPECT_CALL(view, set_skybox_distance(skybox_distance)).Times(1);
    EXPECT_CALL(view, set_ambient_color({ { 1, 1, 1, 0 } })).Times(1);
    EXPECT_CALL(view, set_diffuse_color({ { 13, 11, 12, 0 } })).Times(1);
    EXPECT_CALL(view, set_specular_color({ { 1, 31, 10, 0 } })).Times(1);
    EXPECT_CALL(view, set_cam_far_clip_distance(cam_far_clip_distance))
        .Times(1);
    EXPECT_CALL(view, set_cam_near_clip_distance(cam_near_clip_distance))
        .Times(1);

    pres->select_restore();
}

} // namespace