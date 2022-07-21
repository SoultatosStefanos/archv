#include "features/layout/presenter.hpp"
#include "utility/all.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace
{

using namespace dependencies;
using namespace utility;
using namespace features::layout;

namespace lay = features::layout;

class iview
{
public:
    iview() = default;
    iview(const iview&) = default;
    iview(iview&&) = default;
    virtual ~iview() = default;

    auto operator=(const iview&) -> iview& = default;
    auto operator=(iview&&) -> iview& = default;

    virtual void draw_vertex(const std::string&, double, double, double) = 0;
    virtual void update_layout_selection(const std::string&) = 0;
    virtual void update_topology_selection(const std::string&, double) = 0;

    virtual void
    on_layout_request(const std::function<void(const std::string&)>&) = 0;

    virtual void on_topology_request(
        const std::function<void(const std::string&, double)>&) = 0;
};

class mock_view : public iview
{
public:
    mock_view() = default;
    ~mock_view() override = default;

    MOCK_METHOD(void,
                draw_vertex,
                (const std::string&, double, double, double),
                (override));

    MOCK_METHOD(void,
                update_layout_selection,
                (const std::string&),
                (override));

    MOCK_METHOD(void,
                update_topology_selection,
                (const std::string&, double),
                (override));

    MOCK_METHOD(void,
                on_layout_request,
                (const std::function<void(const std::string&)>&),
                (override));

    MOCK_METHOD(void,
                on_topology_request,
                (const std::function<void(const std::string&, double)>&),
                (override));
};

class iupdate_layout
{
public:
    iupdate_layout() = default;
    iupdate_layout(const iupdate_layout&) = default;
    iupdate_layout(iupdate_layout&&) = default;
    virtual ~iupdate_layout() = default;

    auto operator=(const iupdate_layout&) -> iupdate_layout& = default;
    auto operator=(iupdate_layout&&) -> iupdate_layout& = default;

    virtual void on_layout_response(
        const std::function<void(const lay::layout&, const topology&)>&) = 0;

    virtual void execute(const layout_factory::descriptor&) = 0;
};

class mock_update_layout : public iupdate_layout
{
public:
    mock_update_layout() = default;
    ~mock_update_layout() override = default;

    MOCK_METHOD(
        void,
        on_layout_response,
        (const std::function<void(const lay::layout&, const topology&)>&),
        (override));

    MOCK_METHOD(void, execute, (const layout_factory::descriptor&), (override));
};

class iupdate_topology
{
public:
    iupdate_topology() = default;
    iupdate_topology(const iupdate_topology&) = default;
    iupdate_topology(iupdate_topology&&) = default;
    virtual ~iupdate_topology() = default;

    auto operator=(const iupdate_topology&) -> iupdate_topology& = default;
    auto operator=(iupdate_topology&&) -> iupdate_topology& = default;

    virtual void on_layout_response(
        const std::function<void(const lay::layout&, const topology&)>&) = 0;

    virtual void execute(const layout_factory::descriptor&,
                         topology_factory::scale_type) = 0;
};

class mock_update_topology : public iupdate_topology
{
public:
    mock_update_topology() = default;
    ~mock_update_topology() override = default;

    MOCK_METHOD(
        void,
        on_layout_response,
        (const std::function<void(const lay::layout&, const topology&)>&),
        (override));

    MOCK_METHOD(void,
                execute,
                (const layout_factory::descriptor&,
                 topology_factory::scale_type),
                (override));
};

class A_layout_presenter : public testing::Test
{
public:
    using test_presenter = presenter<testing::NiceMock<mock_view>,
                                     testing::NiceMock<mock_update_layout>,
                                     testing::NiceMock<mock_update_topology>>;

    void SetUp() override
    {
        instance =
            std::make_unique<test_presenter>(g,
                                             mocked_view,
                                             mocked_update_layout_service,
                                             mocked_update_topology_service);
    }

protected:
    static constexpr auto vertices_num = 100;

    graph g{vertices_num};
    testing::NiceMock<mock_view> mocked_view;
    testing::NiceMock<mock_update_layout> mocked_update_layout_service;
    testing::NiceMock<mock_update_topology> mocked_update_topology_service;
    std::unique_ptr<test_presenter> instance;
};

TEST_F(A_layout_presenter,
       Given_a_layout_and_topology_draws_each_vertex_on_view)
{
    const auto space = cube(10);
    const auto lay =
        gursoy_atun_layout(g, space, weight_map([](auto) { return 1; }));

    EXPECT_CALL(mocked_view,
                draw_vertex(testing::_, testing::_, testing::_, testing::_))
        .Times(vertices_num);

    instance->update_view(lay, space);
}

TEST_F(A_layout_presenter,
       Given_a_layout_and_topology_sets_the_selected_elements_at_view)
{
    const auto space = cube(10);
    const auto lay =
        gursoy_atun_layout(g, space, weight_map([](auto) { return 1; }));

    EXPECT_CALL(mocked_view, update_layout_selection(lay.desc()));
    EXPECT_CALL(mocked_view, update_topology_selection(space.desc(), 10));

    instance->update_view(lay, space);
}

TEST_F(A_layout_presenter, Routes_update_layout_requests)
{
    EXPECT_CALL(mocked_update_layout_service,
                execute(layout_factory::gursoy_atun_desc));

    instance->update_layout(layout_factory::gursoy_atun_desc);
}

TEST_F(A_layout_presenter, Routes_update_topology_requests)
{
    EXPECT_CALL(mocked_update_topology_service,
                execute(topology_factory::cube_desc, 1000000));

    instance->update_topology(topology_factory::cube_desc, 1000000);
}

} // namespace