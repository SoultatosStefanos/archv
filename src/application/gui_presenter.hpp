// Contains the class which hooks gui widgets.
// Soultatos Stefanos 2022

#ifndef APPLICATION_GUI_PRESENTER_HPP
#define APPLICATION_GUI_PRESENTER_HPP

#include "boost/log/trivial.hpp"
#include "gui/gui.hpp"
#include "ui/ui_component.hpp"

namespace application
{

template <
    typename GUI = gui::gui,
    typename Configurator = gui::gui_configurator >
class gui_pesenter
{
public:
    using gui_type = GUI;
    using gui_config_data_type = typename gui_type::config_data_type;
    using gui_name_type = typename gui_config_data_type::name_type;
    using gui_rounding_type = typename gui_config_data_type::rounding_type;
    using gui_bordered_type = typename gui_config_data_type::bordered_type;

    using configurator_type = Configurator;
    using configurator_name_type = typename Configurator::name_type;
    using configurator_rounding_type = typename Configurator::rounding_type;
    using configurator_bordered_type = typename Configurator::bordered_type;

    gui_presenter(gui_type& root, configurator_type& configurator)
    : m_root { root }, m_configurator { configurator }
    {
        initialize_configurator();
        install_configurator_responses();
    }

    auto root() const -> const auto& { return m_root; }
    auto configurator() const -> const auto& { return m_configurator; }

    auto fetch_color_theme() -> void
    {
        m_configurator.set_color_theme(m_root.config_data().color_theme);
        BOOST_LOG_TRIVIAL(info) << "fetched gui color theme";
    }

    auto fetch_frame_rounding() -> void
    {
        m_configurator.set_frame_rounding(m_root.config_data().frame_rounding);
        BOOST_LOG_TRIVIAL(info) << "fetched gui frame rounding";
    }

    auto fetch_frame_bordered() -> void
    {
        m_configurator.set_frame_bordered(m_root.config_data().frame_bordered);
        BOOST_LOG_TRIVIAL(info) << "fetched gui frame bordered";
    }

    auto fetch_window_bordered() -> void
    {
        m_configurator.set_frame_bordered(m_root.config_data().window_bordered);
        BOOST_LOG_TRIVIAL(info) << "fetched gui window bordered";
    }

    auto fetch_popup_bordered() -> void
    {
        m_configurator.set_frame_bordered(m_root.config_data().popup_bordered);
        BOOST_LOG_TRIVIAL(info) << "fetched gui popup bordered";
    }

    auto select_color_theme(configurator_name_type theme) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui color theme: " << theme;
        m_root.config_api().set_color_theme(translate(theme));
    }

    auto select_frame_rounding(configurator_rounding_type r) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui frame rounding: " << r;
        m_root.config_api().set_frame_rounding(r);
    }

    auto select_window_bordered(configurator_bordered_type v) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui window bordered: " << v;
        m_root.config_api().set_window_bordered(v);
    }

    auto select_frame_bordered(configurator_bordered_type v) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui frame bordered: " << v;
        m_root.config_api().set_frame_bordered(v);
    }

    auto select_popup_bordered(configurator_bordered_type v) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui popup bordered: " << v;
        m_root.config_api().set_popup_bordered(v);
    }

    auto select_apply() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui apply";
        ui::apply_configs(m_root);
    }

    auto select_preview() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui preview";
        ui::begin_preview(m_root);
    }

    auto select_cancel() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui cancel";
        ui::end_preview(m_root);
    }

    auto select_restore() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected gui restore";
        ui::restore_defaults(m_root);
        initialize_configurator();
    }

protected:
    static inline auto translate(configurator_name_type name)
    {
        static_assert(
            std::is_constructible_v< gui_name_type, configurator_name_type >);
        return gui_name_type(name);
    }

private:
    auto initialize_configurator() -> void
    {
        fetch_color_theme();
        fetch_frame_rounding();
        fetch_frame_bordered();
        fetch_window_bordered();
        fetch_popup_bordered();

        BOOST_LOG_TRIVIAL(debug) << "initialized gui configurator";
    }

    auto install_configurator_responses() -> void
    {
        m_configurator.connect_to_color_theme([this](auto theme)
                                              { select_color_theme(theme); });

        m_configurator.connect_to_frame_rounding([this](auto r)
                                                 { select_frame_rounding(r); });

        m_configurator.connect_to_window_bordered(
            [this](auto v) { select_window_bordered(v); });

        m_configurator.connect_to_frame_bordered([this](auto v)
                                                 { select_frame_bordered(v); });

        m_configurator.connect_to_popup_bordered([this](auto v)
                                                 { select_popup_bordered(v); });

        m_configurator.connect_to_apply([this]() { select_apply(); });
        m_configurator.connect_to_preview([this]() { select_preview(); });
        m_configurator.connect_to_cancel([this]() { select_cancel(); });
        m_configurator.connect_to_restore([this]() { select_restore(); });

        BOOST_LOG_TRIVIAL(debug) << "installed gui configurator responses";
    }

    gui_type& m_root;
    configurator_type& m_configurator;
};

} // namespace application

#endif // APPLICATION_GUI_PRESENTER_HPP
