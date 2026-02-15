#pragma once
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

    // Cleanup any core resources
    void Shutdown();

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