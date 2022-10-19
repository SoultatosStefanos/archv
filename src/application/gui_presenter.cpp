#include "gui_presenter.hpp"

#include "gui/all.hpp"
#include "ui/ui_component.hpp"

#include <boost/log/trivial.hpp>

namespace application
{

gui_presenter::gui_presenter(gui_type& root) : m_gui { root }
{
    prepare_configurator();
    connect_configurator_with_gui();
}

auto gui_presenter::prepare_configurator() -> void
{
    auto& configurator = root().get_configurator().get_gui_configurator();
    const auto& cfg = root().config_data();

    configurator.set_color_theme(cfg.color_theme);
    configurator.set_frame_rounding(cfg.frame_rounding);
    configurator.set_frame_bordered(cfg.frame_bordered);
    configurator.set_window_bordered(cfg.window_bordered);
    configurator.set_popup_bordered(cfg.popup_bordered);

    BOOST_LOG_TRIVIAL(debug) << "prepared gui configurator";
}

auto gui_presenter::connect_configurator_with_gui() -> void
{
    root().get_configurator().get_gui_configurator().connect_to_color_theme(
        [this](auto theme)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui color theme: " << theme;
            m_gui->config_api().config_data().color_theme = std::string(theme);
        });

    root().get_configurator().get_gui_configurator().connect_to_frame_rounding(
        [this](auto r)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame rounding: " << r;
            m_gui->config_api().config_data().frame_rounding = r;
        });

    root().get_configurator().get_gui_configurator().connect_to_window_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui window bordered: " << b;
            m_gui->config_api().config_data().window_bordered = b;
        });

    root().get_configurator().get_gui_configurator().connect_to_frame_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui frame bordered: " << b;
            m_gui->config_api().config_data().frame_bordered = b;
        });

    root().get_configurator().get_gui_configurator().connect_to_popup_bordered(
        [this](auto b)
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui popup bordered: " << b;
            m_gui->config_api().config_data().popup_bordered = b;
        });

    root().get_configurator().get_gui_configurator().connect_to_apply(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs apply";
            ui::apply_configs(root());
        });

    root().get_configurator().get_gui_configurator().connect_to_preview(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs preview";
            ui::begin_preview(root());
        });

    root().get_configurator().get_gui_configurator().connect_to_cancel(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs cancel";
            ui::end_preview(root());
        });

    root().get_configurator().get_gui_configurator().connect_to_restore(
        [this]()
        {
            BOOST_LOG_TRIVIAL(info) << "selected gui configs restore";
            ui::restore_defaults(root());
        });

    BOOST_LOG_TRIVIAL(debug) << "connected gui configurator with gui";
}

} // namespace application