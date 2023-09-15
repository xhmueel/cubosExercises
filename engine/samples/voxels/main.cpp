#include <cubos/core/settings.hpp>

#include <cubos/engine/env_settings/plugin.hpp>
#include <cubos/engine/renderer/light.hpp>
#include <cubos/engine/renderer/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>
#include <cubos/engine/voxels/plugin.hpp>

using cubos::core::Settings;
using cubos::core::ecs::Commands;
using cubos::core::ecs::Read;
using cubos::core::ecs::Write;
using cubos::core::gl::Grid;
using cubos::core::gl::Palette;
using namespace cubos::engine;

/// [Get handles to assets]
static const Asset<Grid> CarAsset = AnyAsset("059c16e7-a439-44c7-9bdc-6e069dba0c75");
static const Asset<Palette> PaletteAsset = AnyAsset("1aa5e234-28cb-4386-99b4-39386b0fc215");
/// [Get handles to assets]

static void settingsSystem(Write<Settings> settings)
{
    settings->setString("assets.io.path", SAMPLE_ASSETS_FOLDER);
}

/// [Load and set palette]
static void setPaletteSystem(Read<Assets> assets, Write<Renderer> renderer)
{
    // Get a handle to the palette asset.
    auto palette = assets->read(PaletteAsset);

    // Set the renderer's palette.
    (*renderer)->setPalette(*palette);
}
/// [Load and set palette]

static void spawnCameraSystem(Commands cmds, Write<ActiveCameras> activeCameras)
{
    // Spawn the camera entity.
    activeCameras->entities[0] =
        cmds.create()
            .add(Camera{.fovY = 60.0F, .zNear = 0.1F, .zFar = 1000.0F})
            .add(LocalToWorld{})
            .add(Position{{0.0F, 120.0F, -200.0F}})
            .add(Rotation{glm::quatLookAt(glm::normalize(glm::vec3{0.0F, -1.0F, 1.0F}), glm::vec3{0.0F, 1.0F, 0.0F})})
            .entity();
}

static void spawnLightSystem(Commands cmds)
{
    // Spawn the sun.
    cmds.create()
        .add(DirectionalLight{glm::vec3(1.0F), 1.0F})
        .add(LocalToWorld{})
        .add(Rotation{glm::quat(glm::vec3(glm::radians(45.0F), glm::radians(45.0F), 0))});
}

/// [Spawn car system]
static void spawnCarSystem(Commands cmds, Read<Assets> assets)
{
    auto car = assets->read(CarAsset);
    glm::vec3 offset = glm::vec3(car->size().x, 0.0F, car->size().z) / -2.0F;

    // Create the car entity
    cmds.create()
        .add(RenderableGrid{CarAsset, offset})
        .add(LocalToWorld{})
        .add(Position{{0.0F, 0.0F, -60.0F}})
        .add(Rotation{glm::quat(glm::vec3(0, glm::radians(90.0F), 0))});
}
/// [Spawn car system]

int main(int argc, char** argv)
{
    Cubos cubos{argc, argv};
    cubos.addPlugin(envSettingsPlugin);

    cubos.addPlugin(rendererPlugin);
    /// [Adding the plugin]
    cubos.addPlugin(voxelsPlugin);
    /// [Adding the plugin]

    cubos.startupSystem(settingsSystem).tagged("cubos.settings");
    cubos.startupSystem(spawnCameraSystem);
    cubos.startupSystem(spawnLightSystem);
    /// [Adding systems]
    cubos.startupSystem(setPaletteSystem).after("cubos.renderer.init");
    cubos.system(spawnCarSystem);
    /// [Adding systems]

    cubos.run();
}