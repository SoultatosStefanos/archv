#include "utility/all.hpp"
#include "visualization/layout/view.hpp"

#include <Ogre.h>
#include <OgreRoot.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace
{

using namespace visualization::layout;
using namespace visualization::communication;
using namespace utility;
using namespace Ogre;

class mock_scene : public SceneManager
{
public:
    mock_scene(const char* name) : SceneManager(name) {}
    virtual ~mock_scene() = default;

    MOCK_METHOD(const String&, getTypeName, (), (const, override));
};

class A_layout_view : public testing::Test
{
public:
    static constexpr auto node_id = "abcdefg";

    void SetUp() override
    {
        pipeline = std::make_unique<event_bus>();
        root = std::make_unique<Root>("", "", "");
        scene = root->createSceneManager();
        scene->getRootSceneNode()->createChildSceneNode(node_id);
        v = std::make_unique<view>(*pipeline, *scene);
    }

    void TearDown() override
    {
        scene->clearScene();
        root->destroySceneManager(scene);
    }

protected:
    std::unique_ptr<event_bus> pipeline;
    SceneManager* scene{nullptr};
    std::unique_ptr<Root> root;
    std::unique_ptr<view> v;
};

TEST_F(A_layout_view, Positions_each_scene_node_according_to_the_layout_data)
{
    double x = 10, y = 2, z = -1;

    v->draw_vertex(node_id, x, y, z);

    ASSERT_EQ(scene->getSceneNode(node_id)->getPosition(), Vector3(x, y, z));
}

} // namespace