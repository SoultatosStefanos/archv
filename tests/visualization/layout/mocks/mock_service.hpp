#ifndef MOCKS_MOCK_SERVICE_HPP
#define MOCKS_MOCK_SERVICE_HPP

#include "visualization/layout/layout.hpp"

#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

namespace visualization::layout::mocks
{

class layout_service
{
public:
    using on_layout_changed = std::function<void(const layout&)>;

    layout_service() = default;
    layout_service(const layout_service&) = default;
    layout_service(layout_service&&) = default;
    virtual ~layout_service() = default;

    auto operator=(const layout_service&) -> layout_service& = default;
    auto operator=(layout_service&&) -> layout_service& = default;

    virtual void update_layout(const std::string& type) = 0;
    virtual void update_topology(const std::string& type, double scale) = 0;

    virtual void add_layout_changed_listener(on_layout_changed f) = 0;
};

class mock_layout_service : public layout_service
{
public:
    virtual ~mock_layout_service() override = default;

    MOCK_METHOD(void, update_layout, (const std::string&), (override));

    MOCK_METHOD(void,
                update_topology,
                (const std::string&, double),
                (override));

    MOCK_METHOD(void,
                add_layout_changed_listener,
                (on_layout_changed),
                (override));
};

} // namespace visualization::layout::mocks

#endif // MOCKS_MOCK_SERVICE_HPP
