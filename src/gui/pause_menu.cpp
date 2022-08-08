#include "pause_menu.hpp"

#include <cassert>

namespace gui
{

namespace
{

    auto native_gui() -> auto&
    {
        assert(MyGUI::Gui::getInstancePtr());
        return MyGUI::Gui::getInstance();
    }

} // namespace

pause_menu::pause_menu(
    layout_options layouts, topology_options topologies, scale_options scales)
: m_layouts { std::move(layouts) }
, m_topologies { topologies }
, m_scales { scales }
{
    /***********************************************************
     * Menu bar                                                *
     ***********************************************************/

    m_root = native_gui().createWidget< MyGUI::MenuBar >(
        "MenuBar",
        20,
        40,
        1880,
        60,
        MyGUI::Align::HStretch | MyGUI::Align::Top,
        "Main");
    m_widgets.push_back(m_root);

    /***********************************************************
     * Layout/Topology                                         *
     ***********************************************************/

    auto* layout_topology_btn
        = m_root->addItem("Layout / Topology", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(layout_topology_btn);

    auto* layout_topology_popup
        = layout_topology_btn->createWidget< MyGUI::PopupMenu >(
            MyGUI::WidgetStyle::Popup,
            "PopupMenu",
            MyGUI::IntCoord(),
            MyGUI::Align::Default,
            "Popup");
    m_widgets.push_back(layout_topology_popup);

    layout_topology_popup->setPopupAccept(true);

    /***********************************************************
     * Layout                                                  *
     ***********************************************************/

    auto* layout_btn
        = layout_topology_popup->addItem("Layout", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(layout_btn);

    auto* layout_popup = layout_btn->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");
    m_widgets.push_back(layout_popup);

    layout_popup->setPopupAccept(true);

    for (const auto& option : m_layouts)
    {
        auto* i = layout_popup->addItem(option, MyGUI::MenuItemType::Normal);
        m_widgets.push_back(i);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_layout);
    }

    /***********************************************************
     * Topology                                                *
     ***********************************************************/

    auto* topology_btn = layout_topology_popup->addItem(
        "Topology", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(topology_btn);

    auto* topology_popup = topology_btn->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");
    m_widgets.push_back(topology_popup);

    topology_popup->setPopupAccept(true);

    for (const auto& option : m_topologies)
    {
        auto* i = topology_popup->addItem(option, MyGUI::MenuItemType::Normal);
        m_widgets.push_back(i);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_topology);
    }

    /***********************************************************
     * Scale                                                   *
     ***********************************************************/

    auto* scale_btn
        = layout_topology_popup->addItem("Scale", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(scale_btn);

    auto* scale_popup = scale_btn->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");
    m_widgets.push_back(scale_popup);

    scale_popup->setPopupAccept(true);

    for (auto option : m_scales)
    {
        const auto scale_str = std::to_string(option);
        auto* i = scale_popup->addItem(scale_str, MyGUI::MenuItemType::Normal);
        m_widgets.push_back(i);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_scale);
    }

    /***********************************************************
     * Clustering                                              *
     ***********************************************************/

    auto* clustering_btn
        = m_root->addItem("Clustering", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(clustering_btn);

    /***********************************************************
     * Code inspection                                         *
     ***********************************************************/

    auto* code_inspection_btn
        = m_root->addItem("Code Inspection", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(code_inspection_btn);

    /***********************************************************
     * Settings                                                *
     ***********************************************************/

    auto* settings_btn
        = m_root->addItem("Settings", MyGUI::MenuItemType::Popup);
    m_widgets.push_back(settings_btn);

    /***********************************************************
     * Separators                                              *
     ***********************************************************/

    auto* s1 = m_root->addItem("Seperator1", MyGUI::MenuItemType::Separator);
    m_widgets.push_back(s1);
    auto* s2 = m_root->addItem("Seperator2", MyGUI::MenuItemType::Separator);
    m_widgets.push_back(s2);
    auto* s3 = m_root->addItem("Seperator3", MyGUI::MenuItemType::Separator);
    m_widgets.push_back(s3);
    auto* s4 = m_root->addItem("Seperator4", MyGUI::MenuItemType::Separator);
    m_widgets.push_back(s4);

    /***********************************************************
     * Coordinates                                             *
     ***********************************************************/

    layout_topology_btn->setCoord(0, 0, 270, 60);
    clustering_btn->setCoord(270, 0, 270, 60);
    code_inspection_btn->setCoord(540, 0, 270, 60);
    settings_btn->setCoord(1630, 0, 270, 60);

    s1->setCoord(270, 10, 40, 40);
    s2->setCoord(540, 10, 40, 40);
    s3->setCoord(810, 10, 40, 40);
    s4->setCoord(1630, 10, 40, 40);
}

pause_menu::~pause_menu() { native_gui().destroyWidgets(m_widgets); }

void pause_menu::show() { m_root->setVisible(true); }

void pause_menu::hide() { m_root->setVisible(false); }

void pause_menu::emit_layout(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);

    m_layout_sig(item->getCaption());
}

void pause_menu::emit_topology(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);

    m_topology_sig(item->getCaption());
}

void pause_menu::emit_scale(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);
    const auto scale = std::stod(item->getCaption().asUTF8());

    m_scale_sig(scale);
}

} // namespace gui