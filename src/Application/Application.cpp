<<<<<<< HEAD
#include "Application.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <Graphics/Texture.h>
#include <stb_image.h>

#ifdef __APPLE__
#include <mach-o/getsect.h>

#define EXTLD(NAME) \
    extern const unsigned char _section$__DATA__##NAME[];
#define LDVAR(NAME) _section$__DATA__##NAME
#define LDLEN(NAME) (getsectbyname("__DATA", "__" #NAME)->size)

#elif (defined _WINDOWS) /* mingw */
=======
#include "Application/Application.h"
#include "Program/Program.h"
#include "UI/MenuFunctions.h"

// just for the sake of making things a little clearer
DefaultApp* gApp = new DefaultApp();

template class Application<DefaultPlatform, DefaultGraphics, DefaultUI>;

template<typename TPlatform, typename TGraphics, typename TUI>
PlatformBackend* Application<TPlatform, TGraphics, TUI>::GetPlatformBackend()
{
    return mPlatformBackend.get();
}

template<typename TPlatform, typename TGraphics, typename TUI>
RendererBackend* Application<TPlatform, TGraphics, TUI>::GetRendererBackend()
{
    return mRendererBackend.get();
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::SetPlatformBackend(std::unique_ptr<PlatformBackend> backend) {
    mPlatformBackend = std::move(backend);
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::SetRendererBackend(std::unique_ptr<RendererBackend> backend) {
    mRendererBackend = std::move(backend);
}

template<typename TPlatform, typename TGraphics, typename TUI>
bool Application<TPlatform, TGraphics, TUI>::Init(int argc, char* argv[]) {
    mPlatform = std::make_unique<TPlatform>();
    mGraphics = std::make_unique<TGraphics>();
    mUI = std::make_unique<TUI>();
    mInstance = std::make_unique<ProgramInstance>();

    if (argc > 1) {
        OpenPCKFile(argv[1]);
    }
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

#define EXTLD(NAME)                                          \
    extern "C" const unsigned char _binary_##NAME##_start[]; \
    extern "C" const unsigned char _binary_##NAME##_end[]
    // extern "C" const unsigned int _binary_##NAME##_size
#define LDVAR(NAME) \
    _binary_##NAME##_start
#define LDLEN(NAME) \
   ((_binary_##NAME##_end) - (_binary_##NAME##_start))

#else /* gnu/linux ld */

#define EXTLD(NAME)                                          \
    extern "C" const unsigned char _binary_##NAME##_start[]; \
    extern "C" const unsigned char _binary_##NAME##_end[];
#define LDVAR(NAME) \
    _binary_##NAME##_start
#define LDLEN(NAME) \
    ((_binary_##NAME##_end) - (_binary_##NAME##_start))
#endif

static Application *gApp = nullptr;
EXTLD(assets_platforms_windows_pckpp_png);

Application &Application::Get()
{
    return *gApp;
}

<<<<<<< HEAD
Application::Application()
{
    gApp = this;
}

bool Application::Initialize(ApplicationSpecification &spec)
{
    if (mInitialized)
    {
        DBG_LOG("Application already initialized.");
        return true;
    }
    std::filesystem::current_path(SDL_GetBasePath());

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        DBG_LOG("SDL_Init failed.");
        return false;
    }

    mWindow = SDL_CreateWindow(spec.title, spec.width, spec.height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!mWindow)
    {
        DBG_LOG("SDL_CreateWindow failed.");
        return false;
    }

    mContext = SDL_GL_CreateContext(mWindow);
    if (!SDL_GL_SetSwapInterval(1))
    {
        DBG_LOG("SDL_GL_SetSwapInterval failed.");
        return false;
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3))
        return false;
    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3))
        return false;

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY))
        return false;

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
    {
        return false;
    }

    if (!SDL_GL_MakeCurrent(mWindow, mContext))
    {
        return false;
    }

    ImGui::CreateContext();

    const uint8_t *data = LDVAR(assets_platforms_windows_pckpp_png);
    size_t s = LDLEN(assets_platforms_windows_pckpp_png);
    int w, h;
    uint8_t *pixles = stbi_load_from_memory(data, s, &w, &h, nullptr, 4);
    if (!pixles)
    {
        DBG_LOG("icon load failed!");
    }
    SDL_Surface *icon = SDL_CreateSurfaceFrom(w, h, SDL_PIXELFORMAT_ARGB8888, pixles, w*4);

    if (!icon)
    {
        DBG_LOG("%s", SDL_GetError());
    }
    SDL_SetWindowIcon(mWindow, icon);

    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplSDL3_InitForOpenGL(mWindow, mContext);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#if _WINDOWS && USE_VIEWPORTS
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif

    mInitialized = true;
    return true;
}

void Application::ParseCommandLineArguments(int argc, const char **argv)
{
    return;
}

void Application::Run()
{
    int display_w, display_h;
    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
        glEnable(GL_FRAMEBUFFER_SRGB);

        for (auto &layer : mLayers)
        {
            layer->OnUpdate();
        }

        glDisable(GL_FRAMEBUFFER_SRGB);

        ImGui::Render();
        SDL_GetWindowSize(mWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#if _WINDOWS && USE_VIEWPORTS
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(mWindow, context);
        }
#endif
        SDL_GL_SwapWindow(mWindow);
    }
=======
template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::Update() {
    HandleInput();
    HandleMenuBar();
    HandleFileTree();
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::Shutdown() {
    for (auto& [type, tex] : mFileIcons)
        if (tex.id != 0)
            mGraphics->DeleteTexture(tex);

    mFileIcons.clear();
}

template<typename TPlatform, typename TGraphics, typename TUI>
TPlatform* Application<TPlatform, TGraphics, TUI>::GetPlatform() const
{
    return mPlatform.get();
}

template<typename TPlatform, typename TGraphics, typename TUI>
ProgramInstance* Application<TPlatform, TGraphics, TUI>::GetInstance() const
{
    return mInstance.get();
}

template<typename TPlatform, typename TGraphics, typename TUI>
TGraphics* Application<TPlatform, TGraphics, TUI>::GetGraphics() const
{
    return mGraphics.get();
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
}

template<typename TPlatform, typename TGraphics, typename TUI>
TUI* Application<TPlatform, TGraphics, TUI>::GetUI() const
{
<<<<<<< HEAD
    for (auto &l : mLayers)
    {
        l->OnDetach();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DestroyContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
=======
    return mUI.get();
}

template<typename TPlatform, typename TGraphics, typename TUI>
const Texture& Application<TPlatform, TGraphics, TUI>::GetFileIcon(PCKAssetFile::Type type) const {
    static Texture empty{};
    auto it = mFileIcons.find(type);
    return it != mFileIcons.end() ? it->second : empty;
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::SetFileIcon(PCKAssetFile::Type type, const Texture& texture) {
    mFileIcons[type] = texture;
}

template<typename TPlatform, typename TGraphics, typename TUI>
const Texture& Application<TPlatform, TGraphics, TUI>::GetFolderIcon() const {
    return mFolderIcon;
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::SetFolderIcon(const Texture& texture) {
    mFolderIcon = texture;
}

template<typename TPlatform, typename TGraphics, typename TUI>
const Texture& Application<TPlatform, TGraphics, TUI>::GetPreviewTexture() const {
    return mPreviewTexture;
}

template<typename TPlatform, typename TGraphics, typename TUI>
void Application<TPlatform, TGraphics, TUI>::SetPreviewTexture(const Texture& texture) {
    mPreviewTexture = texture;
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
}