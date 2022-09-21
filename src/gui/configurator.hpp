// Contains the configurator gui of the application.
// Soultatos Stefanos 2022

#ifndef GUI_CONFIGURATOR_HPP
#define GUI_CONFIGURATOR_HPP

#include "background_configurator.hpp"
#include "graph_configurator.hpp"
#include "gui_configurator.hpp"

namespace gui
{

class configurator
{
public:
    auto render() const -> void;

    auto get_background_configurator() const -> const auto&
    {
        return m_bkg_cfg;
    }

    auto get_background_configurator() -> auto& { return m_bkg_cfg; }

    auto get_graph_configurator() const -> const auto& { return m_graph_cfg; }
    auto get_graph_configurator() -> auto& { return m_graph_cfg; }

    auto get_gui_configurator() const -> const auto& { return m_gui_cfg; }
    auto get_gui_configurator() -> auto& { return m_gui_cfg; }

private:
    auto render_rendering_configurator() const -> void;
    auto render_gui_configurator() const -> void;

    background_configurator m_bkg_cfg;
    graph_configurator m_graph_cfg;
    gui_configurator m_gui_cfg;
};

} // namespace gui

#endif // GUI_CONFIGURATOR_HPP
