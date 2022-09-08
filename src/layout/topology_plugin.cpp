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
        explicit topology_identifier(std::string& id) : m_id { id } { }
        ~topology_identifier() override = default;

        auto visit(const cube&) const -> void override { m_id = cube_id; }

        auto visit(const sphere&) const -> void override { m_id = sphere_id; }

    private:
        std::string& m_id;
    };

} // namespace

auto identify(const topology& space) -> std::string
{
    std::string res;
    space.accept(topology_identifier(res));
    assert(is_topology_plugged_in(res));
    return res;
}

} // namespace layout
