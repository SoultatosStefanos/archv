#include "topology_plugin.hpp"

#include "cube.hpp"
#include "sphere.hpp"
#include "topology.hpp"
#include "topology_visitor.hpp"

#include <cassert>

namespace layout
{

namespace
{
    class topology_identifier : public topology_visitor
    {
    public:
        using identifier = topology_plugin::identifier;

        explicit topology_identifier(identifier& id) : m_id { id } { }
        ~topology_identifier() override = default;

        auto visit(const cube&) const -> void override
        {
            m_id = topology_plugin::cube_id;
        }

        auto visit(const sphere&) const -> void override
        {
            m_id = topology_plugin::sphere_id;
        }

    private:
        identifier& m_id;
    };

} // namespace

auto topology_plugin::identify(const topology& space) -> identifier
{
    identifier res;
    space.accept(topology_identifier(res));
    assert(topology_plugin::enumerates(res));
    return res;
}

} // namespace layout
