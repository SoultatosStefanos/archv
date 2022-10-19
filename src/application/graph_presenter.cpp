#include "graph_presenter.hpp"

#include "commands.hpp"
#include "ui/ui_component.hpp"
#include "undo_redo/all.hpp"

#include <OGRE/Ogre.h>
#include <boost/log/trivial.hpp>

namespace application
{

graph_presenter::graph_presenter(
    graph_interface& g,
    graph_renderer_type& renderer,
    dependencies_editor_type& deps_editor,
    layout_editor_type& layout_editor,
    scaling_editor_type& scaling_editor,
    configurator_type& configurator,
    command_history_type& cmds)
: m_g_iface { g }
, m_g_renderer { renderer }
, m_deps_editor { deps_editor }
, m_layout_editor { layout_editor }
, m_scaling_editor { scaling_editor }
, m_configurator { configurator }
, m_cmds { cmds }
{
}

auto graph_presenter::prepare_dependencies_editor() -> void
{
    dependencies_editor().set_weights(
        [&backend = graph_iface().dependencies_backend()](auto dependency)
        { return backend.get_weight_repo().get_weight(dependency); });

    BOOST_LOG_TRIVIAL(debug) << "prepared dependencies editor";
}

auto graph_presenter::prepare_layout_editor() -> void
{
    layout_editor().set_layout(
        [&backend = graph_iface().layout_backend()]()
        { return layout::identify(backend.get_layout()); });

    layout_editor().set_topology(
        [&backend = graph_iface().layout_backend()]()
        { return layout::identify(backend.get_topology()); });

    layout_editor().set_scale([&backend = graph_iface().layout_backend()]()
                              { return backend.get_topology().scale(); });

    BOOST_LOG_TRIVIAL(debug) << "prepared layout editor";
}

auto graph_presenter::prepare_scaling_editor() -> void
{
    scaling_editor().set_dims(
        [&backend = graph_iface().scaling_backend()](auto tag)
        { return scaling::get_factor_dims(backend, tag); });

    scaling_editor().set_baseline(
        [&backend = graph_iface().scaling_backend()](auto tag)
        { return scaling::get_factor_baseline(backend, tag); });

    scaling_editor().set_enabled(
        [&backend = graph_iface().scaling_backend()](auto tag)
        { return scaling::is_factor_enabled(backend, tag); });

    scaling_editor().set_min_ratio(
        [&backend = graph_iface().scaling_backend()](auto tag)
        { return scaling::get_factor_min_ratio(backend, tag); });

    scaling_editor().set_max_ratio(
        [&backend = graph_iface().scaling_backend()](auto tag)
        { return scaling::get_factor_max_ratio(backend, tag); });

    BOOST_LOG_TRIVIAL(debug) << "prepared scaling editor";
}

namespace
{
    inline auto to_rgba(const Ogre::ColourValue& val)
    {
        return std::array { val.r, val.g, val.b, val.a };
    }

    inline auto to_scale(const Ogre::Vector3& vec)
    {
        return std::array { vec.x, vec.y, vec.z };
    }

} // namespace

auto graph_presenter::prepare_configurator() -> void
{
    const auto& cfg = renderer().config_data();

    configurator().set_node_mesh(cfg.vertex_mesh);
    configurator().set_node_scale(to_scale(cfg.vertex_scale));
    configurator().set_node_font(cfg.vertex_id_font_name);
    configurator().set_node_font_color(to_rgba(cfg.vertex_id_color));
    configurator().set_node_char_height(cfg.vertex_id_char_height);
    configurator().set_node_space_width(cfg.vertex_id_space_width);
    configurator().set_edge_material(cfg.edge_material);
    configurator().set_edge_tip_mesh(cfg.edge_tip_mesh);
    configurator().set_edge_tip_scale(to_scale(cfg.edge_tip_scale));
    configurator().set_edge_font(cfg.edge_type_font_name);
    configurator().set_edge_font_color(to_rgba(cfg.edge_type_color));
    configurator().set_edge_char_height(cfg.edge_type_char_height);
    configurator().set_edge_space_width(cfg.edge_type_space_width);

    BOOST_LOG_TRIVIAL(debug) << "prepared graph configurator";
}

auto graph_presenter::connect_dependencies_editor_with_graph_iface() -> void
{
    dependencies_editor().connect_to_dependency(
        [this, &backend = graph_iface().dependencies_backend()](
            auto dependency, auto weight)
        {
            BOOST_LOG_TRIVIAL(info) << "assigned " << weight
                                    << " to: " << dependency << " dependency";

            update_dependency_weight(commands(), backend, dependency, weight);
        });

    dependencies_editor().connect_to_restore(
        [this, &backend = graph_iface().dependencies_backend()]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected dependencies restore";
            restore_dependencies_defaults(commands(), backend);
        });

    BOOST_LOG_TRIVIAL(debug) << "connected dependencies editor with graph";
}

auto graph_presenter::connect_layout_editor_with_graph_iface() -> void
{
    layout_editor().connect_to_layout(
        [this, &backend = graph_iface().layout_backend()](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " layout";
            update_layout(commands(), backend, selection);
        });

    layout_editor().connect_to_topology(
        [this, &backend = graph_iface().layout_backend()](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " topology";
            update_layout_topology(commands(), backend, selection);
        });

    layout_editor().connect_to_scale(
        [this, &backend = graph_iface().layout_backend()](auto selection)
        {
            BOOST_LOG_TRIVIAL(info) << "selected " << selection << " scale";
            layout::update_scale(backend, selection);
#if (0) // FIXME
            update_layout_scale(m_cmds, *m_layout_backend, selection);
#endif
        });

    layout_editor().connect_to_restore(
        [this, &backend = graph_iface().layout_backend()]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected layout restore";
            restore_layout_defaults(commands(), backend);
        });

    BOOST_LOG_TRIVIAL(debug) << "connected layout editor with graph";
}

auto graph_presenter::connect_scaling_editor_with_graph_iface() -> void
{
    scaling_editor().connect_to_baseline(
        [this,
         &backend = graph_iface().scaling_backend()](auto tag, auto baseline)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling factor: " << tag
                                    << ", " << baseline << " baseline";

            update_scaling_factor_baseline(commands(), backend, tag, baseline);
        });

    scaling_editor().connect_to_dims(
        [this,
         &backend = graph_iface().scaling_backend()](auto tag, const auto& dims)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected scaling factor: " << tag << ", " << dims[0] << ", "
                << dims[1] << ", " << dims[2] << " applied dimensions";

            update_scaling_factor_dims(commands(), backend, tag, dims);
        });

    scaling_editor().connect_to_enabled(
        [this,
         &backend = graph_iface().scaling_backend()](auto tag, auto enabled)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling factor: " << tag
                                    << ", " << enabled << " enabled";

            update_scaling_factor_enablement(commands(), backend, tag, enabled);
        });

    scaling_editor().connect_to_min_ratio(
        [this, &backend = graph_iface().scaling_backend()](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling factor: " << tag
                                    << ", " << ratio << " min ratio";

            update_scaling_factor_min_ratio(commands(), backend, tag, ratio);
        });

    scaling_editor().connect_to_max_ratio(
        [this, &backend = graph_iface().scaling_backend()](auto tag, auto ratio)
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling factor: " << tag
                                    << ", " << ratio << " max ratio";

            update_scaling_factor_max_ratio(commands(), backend, tag, ratio);
        });

    scaling_editor().connect_to_restore(
        [this, &backend = graph_iface().scaling_backend()]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected scaling restore";
            restore_scaling_defaults(commands(), backend);
        });

    BOOST_LOG_TRIVIAL(debug) << "connected scaling editor with graph";
}

auto graph_presenter::connect_graph_iface_with_renderer() -> void
{
    graph_iface().dependencies_backend().connect(
        [this](const auto&, auto)
        {
            BOOST_LOG_TRIVIAL(info) << "dependencies rendered";
            renderer().render_weights(graph_iface().edge_weight());
        });

    graph_iface().layout_backend().connect_to_layout(
        [this](const auto&)
        {
            BOOST_LOG_TRIVIAL(info) << "layout rendered";
            renderer().render_layout(graph_iface().vertex_position());
        });

    graph_iface().scaling_backend().connect(
        [this](const auto&)
        {
            BOOST_LOG_TRIVIAL(info) << "scaling rendered";
            renderer().render_scaling(graph_iface().vertex_scale());
        });

    BOOST_LOG_TRIVIAL(debug) << "connected graph with renderer";
}

namespace
{
    inline auto
    to_string(const graph_presenter::configurator_type::scale_type& scale)
    {
        assert(scale.size() == 3);
        return std::to_string(scale[0]) + ", " + std::to_string(scale[1]) + ", "
            + std::to_string(scale[2]);
    }

    inline auto
    to_string(const graph_presenter::configurator_type::rgba_type& rgba)
    {
        assert(rgba.size() == 4);
        return std::to_string(rgba[0]) + ", " + std::to_string(rgba[1]) + ", "
            + std::to_string(rgba[2]) + ", " + std::to_string(rgba[3]);
    }

    inline auto
    from_scale(const graph_presenter::configurator_type::scale_type& scale)
    {
        assert(scale.size() == 3);
        return Ogre::Vector3(scale[0], scale[1], scale[2]);
    }

    inline auto
    from_rgba(const graph_presenter::configurator_type::rgba_type& rgba)
    {
        assert(rgba.size() == 4);
        return Ogre::ColourValue(rgba[0], rgba[1], rgba[2], rgba[3]);
    }

} // namespace

auto graph_presenter::connect_configurator_with_renderer() -> void
{
    configurator().connect_to_node_mesh(
        [this](auto m)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node mesh: " << m;
            renderer().config_api().config_data().vertex_mesh = std::string(m);
        });

    configurator().connect_to_node_scale(
        [this](const auto& s)
        {
            BOOST_LOG_TRIVIAL(info) << "selected node scale: " << to_string(s);
            renderer().config_api().config_data().vertex_scale = from_scale(s);
        });

    configurator().connect_to_node_font(
        [this](auto font)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node font: " << font;
            renderer().config_api().config_data().vertex_id_font_name
                = std::string(font);
        });

    configurator().connect_to_node_font_color(
        [this](const auto& col)
        {
            assert(col.size() == 4);

            BOOST_LOG_TRIVIAL(info)
                << "selected graph node font color: " << to_string(col);

            renderer().config_api().config_data().vertex_id_color
                = from_rgba(col);
        });

    configurator().connect_to_node_char_height(
        [this](auto h)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node char height: " << h;

            renderer().config_api().config_data().vertex_id_char_height = h;
        });

    configurator().connect_to_node_space_width(
        [this](auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph node space width: " << w;

            renderer().config_api().config_data().vertex_id_space_width = w;
        });

    configurator().connect_to_edge_material(
        [this](auto mat)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge material: " << mat;
            renderer().config_api().config_data().edge_material
                = std::string(mat);
        });

    configurator().connect_to_edge_tip_mesh(
        [this](auto mesh)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge tip mesh: " << mesh;
            renderer().config_api().config_data().edge_tip_mesh
                = std::string(mesh);
        });

    configurator().connect_to_edge_tip_scale(
        [this](const auto& scale)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected edge tip scale: " << to_string(scale);

            renderer().config_api().config_data().edge_tip_scale
                = from_scale(scale);
        });

    configurator().connect_to_edge_font(
        [this](auto font)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge font: " << font;

            renderer().config_api().config_data().edge_type_font_name
                = std::string(font);
        });

    configurator().connect_to_edge_font_color(
        [this](const auto& c)
        {
            BOOST_LOG_TRIVIAL(info)
                << "selected graph edge font color: " << to_string(c);

            renderer().config_api().config_data().edge_type_color
                = from_rgba(c);
        });

    configurator().connect_to_edge_char_height(
        [this](auto h)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge char height: " << h;
            renderer().config_api().config_data().edge_type_char_height = h;
        });

    configurator().connect_to_edge_space_width(
        [this](auto w)
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph edge space width: " << w;
            renderer().config_api().config_data().edge_type_space_width = w;
        });

    configurator().connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs apply";
            ui::apply_configs(renderer());
        });

    configurator().connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs preview";
            ui::begin_preview(renderer());
        });

    configurator().connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs cancel";
            ui::end_preview(renderer());
        });

    configurator().connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected graph configs restore";
            ui::restore_defaults(renderer());
        });

    BOOST_LOG_TRIVIAL(debug) << "connected graph configurator with renderer";
}

} // namespace application