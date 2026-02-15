#pragma once
<<<<<<< HEAD
#include <string>
#include <vector>
#include <memory>
#include <imgui.h>
#include "SDL3/SDL.h"
#include "Log.h"
#include <glad/glad.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_opengl3.h>
#include "Layer.h"

struct ApplicationSpecification
{
    const char *title;
    int width;
    int height;
};

class Application
{
public:
    Application();
    ~Application() = default;

    // Initialize core app
    bool Initialize(ApplicationSpecification &spec);

    void ParseCommandLineArguments(int argc, const char **argv);
=======

#include <iostream>
#include <map>
#include "Graphics/GraphicsBase.h"
#include "Platform/PlatformBase.h"
#include "Program/ProgramInstance.h"
#include "UI/UIBase.h"

template<typename TPlatform, typename TGraphics, typename TUI>
class Application {
public:
    static_assert(std::is_base_of<PlatformBase, TPlatform>::value, "TPlatform must derive from PlatformBase");
    static_assert(std::is_base_of<GraphicsBase, TGraphics>::value, "TGraphics must derive from GraphicsBase");
    static_assert(std::is_base_of<UIBase, TUI>::value, "TUI must derive from UIBase");

    Application() = default;
    ~Application() = default;

    // Gets the Platform Backend
    PlatformBackend* GetPlatformBackend();

    // Gets the Renderer Backend
    RendererBackend* GetRendererBackend();

    // Sets the platform backend for the application and UI to use
    void SetPlatformBackend(std::unique_ptr<PlatformBackend> backend);

    // Sets the renderer backend for the application and UI to use
    void SetRendererBackend(std::unique_ptr<RendererBackend> backend);

    // Initialize core app; pass argc/argv if needed for CLI args
    bool Init(int argc, char* argv[]);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

    // Cleanup any core resources
    void Shutdown();

<<<<<<< HEAD
    SDL_Window *GetWindow() const { return mWindow; }

    void Run(void);

    template <class TLayer, typename... Args>
    void AddLayer(Args... args)
    {
        auto l = std::make_unique<TLayer>(args...);
        l->OnAttach();
        mLayers.push_back(std::move(l));
    }

    static Application &Get();

private:
    SDL_Window *mWindow{nullptr};
    SDL_GLContext mContext{nullptr};
    bool mInitialized{false};
    std::vector<std::unique_ptr<Layer>> mLayers;
};
=======
    // Application Update
    void Update();

    ProgramInstance* GetInstance() const;

    // Gets Platform framework from the application
    TPlatform* GetPlatform() const;

    // Gets Graphics framework  from the application
    TGraphics* GetGraphics() const;

    // Gets UI framework from the appliction
    TUI* GetUI() const;

    const Texture& GetFileIcon(PCKAssetFile::Type type) const;
    void SetFileIcon(PCKAssetFile::Type type, const Texture& texture);

    const Texture& GetFolderIcon() const;
    void SetFolderIcon(const Texture& texture);

    const Texture& GetPreviewTexture() const;
    void SetPreviewTexture(const Texture& texture);

private:
    std::unique_ptr<ProgramInstance> mInstance{new ProgramInstance()};
    std::unique_ptr<TPlatform> mPlatform{};
    std::unique_ptr<TGraphics> mGraphics{};
    std::unique_ptr<TUI> mUI{};
    std::unique_ptr<PlatformBackend> mPlatformBackend{};
    std::unique_ptr<RendererBackend> mRendererBackend{};
    bool initialized{ false };

    // Icons for File Tree Nodes, indexed via PCK filetypes
    std::map<PCKAssetFile::Type, Texture> mFileIcons{};
    // Texture for folders
    Texture mFolderIcon{};
    // Current texture for the preview window
    Texture mPreviewTexture{};
};

// Setup your custom application stuff below :3
#include "Platform/PlatformSDL.h"
#include "Graphics/GraphicsOpenGL.h"
#include "UI/UIImGui.h"

using DefaultGraphics = GraphicsOpenGL;
using DefaultUI = UIImGui;
using DefaultPlatform = PlatformSDL;

using DefaultApp = Application<DefaultPlatform, DefaultGraphics, DefaultUI>;

extern DefaultApp* gApp;
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
