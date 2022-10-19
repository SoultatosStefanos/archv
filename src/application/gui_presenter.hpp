// Contains the class which hooks gui widgets.
// Soultatos Stefanos 2022

#ifndef APPLICATION_GUI_PRESENTER_HPP
#define APPLICATION_GUI_PRESENTER_HPP

#include "gui/allfwd.hpp"

namespace application
{

class gui_presenter
{
public:
    using gui_type = gui::gui;

    explicit gui_presenter(gui_type& ui);

    auto root() const -> const gui_type& { return m_gui; }
    auto root() -> gui_type& { return m_gui; }

private:
    auto prepare_configurator() -> void;
    auto connect_configurator_with_gui() -> void;

    gui_type& m_gui;
};

} // namespace application

#endif // APPLICATION_GUI_PRESENTER_HPP
