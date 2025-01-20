#include <osgViewer/Viewer>
#include <osgEarth/EarthManipulator>
#include <osgEarth/ExampleResources>
#include <osgEarth/MapNode>
#include <osgEarthCesium/CesiumCreditsNode>
#include <osgEarth/GUI/ImGuiLayer>

using namespace osgEarth;

class CesiumApp
{
public:
    CesiumApp(osgViewer::Viewer *viewer, osg::Group *root) : _viewer(viewer),
                                                             _root(root)
    {
        // Initialize ImGui layer
        _gui = new GUI::ImGuiLayer();
        _root->addChild(_gui);

        // Setup GUI callbacks
        _gui->addDrawCallback([this]()
                              { this->drawGUI(); });
    }

private:
    void drawGUI()
    {
        ImGui::Begin("Cesium Assets");

        static char assetId[128] = "1"; // Default to Cesium World Terrain
        ImGui::InputText("Asset ID", assetId, sizeof(assetId));

        if (ImGui::Button("Load Asset"))
        {
            // Here you would implement the asset loading logic
            // This is just a placeholder for demonstration
            OSG_NOTICE << "Loading asset ID: " << assetId << std::endl;
        }

        ImGui::End();
    }

    osgViewer::Viewer *_viewer;
    osg::Group *_root;
    GUI::ImGuiLayer *_gui;
};

int main(int argc, char **argv)
{
    osgEarth::initialize();

    osg::ArgumentParser arguments(&argc, argv);
    osgViewer::Viewer viewer(arguments);
    viewer.setCameraManipulator(new EarthManipulator(arguments));

    osg::Group *root = new osg::Group;

    auto node = MapNodeHelper().load(arguments, &viewer);
    if (node.valid())
    {
        root->addChild(node);

        // Add credits node
        auto creditsNode = new osgEarth::Cesium::CesiumCreditsNode(&viewer);
        root->addChild(creditsNode);

        // Create our application
        CesiumApp app(&viewer, root);

        viewer.setSceneData(root);
        return viewer.run();
    }

    return 0;
}