#ifndef MOCKS_MOCK_VIEW_HPP
#define MOCKS_MOCK_VIEW_HPP

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

namespace visualization::mocks
{

class layout_view
{
public:
    using layout_input_listener = std::function<void(const std::string&)>;
    using topology_input_listener =
        std::function<void(const std::string&, double)>;

    layout_view() = default;
    layout_view(const layout_view&) = default;
    layout_view(layout_view&&) = default;
    virtual ~layout_view() = default;

    auto operator=(const layout_view&) -> layout_view& = default;
    auto operator=(layout_view&&) -> layout_view& = default;

    virtual void
    draw_vertex(const std::string& id, double x, double y, double z) const = 0;

    virtual void add_layout_input_listener(layout_input_listener f) = 0;
    virtual void add_topology_input_listener(topology_input_listener f) = 0;
};

class mock_layout_view : public layout_view
{
public:
    virtual ~mock_layout_view() override = default;

    MOCK_METHOD(void,
                draw_vertex,
                (const std::string&, double, double, double),
                (const, override));

    MOCK_METHOD(void,
                add_layout_input_listener,
                (layout_input_listener),
                (override));

    MOCK_METHOD(void,
                add_topology_input_listener,
                (topology_input_listener),
                (override));
};

} // namespace visualization::mocks

#endif // MOCKS_MOCK_VIEW_HPP
