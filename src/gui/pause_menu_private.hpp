// Contains a private module which provides the gui for the pause menu.
// Soultatos Stefanos 2022

#ifndef GUI_PAUSE_MENU_PRIVATE_HPP
#define GUI_PAUSE_MENU_PRIVATE_HPP

#include <string>
#include <vector>

namespace gui::detail
{

// TODO Pass config data?
class pause_menu_gui
{
public:
    using layout_options = std::vector< std::string >;
    using topology_options = std::vector< std::string >;
    using scale_options = std::vector< double >;

    pause_menu_gui(
        layout_options layouts = layout_options(),
        topology_options topologies = topology_options(),
        scale_options scales = scale_options());

    void draw() const;

private:
    layout_options m_layouts;
    topology_options m_topologies;
    scale_options m_scales;
};

} // namespace gui::detail

#endif // GUI_PAUSE_MENU_PRIVATE_HPP
