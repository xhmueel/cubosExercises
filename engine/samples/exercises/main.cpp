#include <cubos/engine/cubos.hpp>

#include <cubos/engine/renderer/plugin.hpp>
#include <cubos/engine/renderer/point_light.hpp>
#include <cubos/engine/renderer/environment.hpp>
#include <cubos/engine/transform/plugin.hpp>

using cubos::core::ecs::Commands;
using cubos::core::ecs::Write;


using namespace cubos::engine;

// Palette
static void setPaletteSystem(Write<Renderer> renderer)
{
    // Create a simple palette with 3 materials (red, green and blue).
    (*renderer)->setPalette(VoxelPalette{{
        {{1, 0, 0, 1}},
        {{0, 1, 0, 1}},
        {{0, 0, 1, 1}},
    }});
}

// Voxel Grid
static void spawnVoxelGridSystem(Commands commands, Write<Assets> assets)
{
    // Create a 2x2x2 grid whose voxels alternate between the materials defined in the palette.
    auto gridAsset = assets->create(VoxelGrid{{2, 2, 2}, {1, 2, 3, 1, 2, 3, 1, 2}});

    // Spawn an entity with a renderable grid component and a identity transform.
    commands.create(RenderableGrid{gridAsset, {-1.0F, 0.0F, -1.0F}}, LocalToWorld{});
}

// Point Light
static void spawnLightSystem(Commands commands)
{
    // Spawn a point light.
    commands.create()
        .add(PointLight{.color = {1.0F, 1.0F, 1.0F}, .intensity = 1.0F, .range = 10.0F})
        .add(Position{{1.0F, 3.0F, -2.0F}});
}

// Camera
static void spawnCamerasSystem(Commands commands, Write<ActiveCameras> camera)
{
    // Spawn the a camera entity for the first viewport.
    camera->entities[0] =
        commands.create()
            .add(Camera{.fovY = 60.0F, .zNear = 0.1F, .zFar = 100.0F})
            .add(Position{{-3.0, 1.0F, -3.0F}})
            .add(Rotation{glm::quatLookAt(glm::normalize(glm::vec3{1.0F, 0.0F, 1.0F}), glm::vec3{0.0F, 1.0F, 0.0F})})
            .entity();

    // Add two other viewports using the same camera, which splits the screen in three.
    camera->entities[1] = camera->entities[0];
    camera->entities[2] = camera->entities[0];
}


int main()
{
    
    Cubos cubos{};

    //Renderer plugin
    cubos.addPlugin(rendererPlugin);

    cubos.startupSystem(setPaletteSystem).after("cubos.renderer.init");
    cubos.startupSystem(spawnVoxelGridSystem);
    cubos.startupSystem(spawnLightSystem);
    cubos.startupSystem(spawnCamerasSystem);

    cubos.run();
}


