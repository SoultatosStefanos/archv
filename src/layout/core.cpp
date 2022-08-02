#include "core.hpp"

#include "cube.hpp"
#include "sphere.hpp"

namespace layout::detail
{

/***********************************************************
 * Topology Factory                                        *
 ***********************************************************/

auto topology_factory::make_topology(const descriptor& desc, scale_type scale)
    -> pointer
{
    if (desc == topology_traits<cube>::desc())
    {
        return std::make_unique<cube>(scale);
    }
    else if (desc == topology_traits<sphere>::desc())
    {
        return std::make_unique<sphere>(scale);
    }
    else
    {
        BOOST_LOG_TRIVIAL(fatal) << "invalid topology description: " << desc;
        assert(false);
        return nullptr;
    }
}

/***********************************************************
 * Revert to defaults Use Case                             *
 ***********************************************************/

} // namespace layout::detail

namespace layout
{

core::core(command_history& cmds,
           const graph& g,
           weight_map edge_weight,
           const std::string& layout_type,
           const std::string& topolgy_type,
           double topology_scale)
    : m_topology{detail::topology_factory::make_topology(topolgy_type,
                                                         topology_scale)},
      m_layout{detail::layout_factory<graph>::make_layout(
          layout_type, g, *m_topology, edge_weight)},
      m_update_layout{cmds, g, edge_weight, m_layout, m_topology},
      m_update_topology{cmds, g, edge_weight, m_layout, m_topology},
      m_revert_to_defaults{cmds, g, edge_weight, m_layout, m_topology}
{
    m_update_layout.connect([this](const auto& l) { m_layout_signal(l); });

    m_update_topology.connect([this](const auto& l, const auto& s) {
        m_layout_signal(l);
        m_topology_signal(s);
    });

    m_revert_to_defaults.connect([this](const auto& l, const auto& s) {
        m_layout_signal(l);
        m_topology_signal(s);
    });
}

void core::update_layout(const std::string& type)
{
    m_update_layout(type);
}

void core::update_topology(const std::string& type, double scale)
{
    m_update_topology(type, scale);
}

void core::revert_to_defaults()
{
    m_revert_to_defaults();
}

} // namespace layout