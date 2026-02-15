#pragma once
#include "Application/Application.h"
#include "Application/Layer.h"

#include "PCK/PCKFile.h"
#include "Log.h"
#include "Graphics/Texture.h"
#include "Menu/MenuFunctions.h"
#include "Tree/TreeFunctions.h"
#include "Math/Vec2Operators.h"
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <memory>
#include <map>

// Setup UI related Vendors
void UISetup();

// Cleanup UI related vendors
void UICleanup();

// Reset PCK++ UI Data; optional filepath passing for when opening the file
void ResetUIData(const std::string &filePath = "");

// Handles the menu bar, functions are held in MenuFunctions.h/cpp
void HandleMenuBar();

// Handle input for the UI, like keystrokes
void HandleInput();

// Instead of writing a 1,000 success messages
void ShowSuccessMessage();

// Instead of writing a 1,000 cancelled messages
void ShowCancelledMessage();

// A helper function to cut cown on work required to open a message prompt box
static int ShowMessagePrompt(const char *title, const char *message, const SDL_MessageBoxButtonData *buttons, int numButtons);

// Basically a helper function to do Yes/No prompts using ShowMessagePromptBox
static bool ShowYesNoMessagePrompt(const char *title, const char *message);

// Handles the file tree, functions are held in TreeFunctions.h/cpp
void HandleFileTree();

class UILayer : public Layer
{
public:
    UILayer()
        : mFileIcons((int)PCKAssetFile::Type::PCK_ASSET_TYPES_TOTAL)
    {
        DBG_LOG("UILayer");
    }

    UILayer(UILayer &)
    {
        DBG_LOG("UILayer(UILayer &)");
    }

    UILayer(UILayer &&)
    {
        DBG_LOG("UILayer(UILayer &&)");
    }

    void OnAttach(void) override;
    void OnUpdate(void) override;
    void OnDetach(void) override;

private:
    std::shared_ptr<PCKFile> mCurrentPCKFile{};
    Texture mFolderIcon{};
    Texture mPreviewTexture{};
    std::string mPreviewTitle = "Preview";
    const PCKAssetFile *mLastPreviewedFile = nullptr;
    std::string mSelectedNodePath;
    std::vector<Texture> mFileIcons;
    FileTreeNode mRootNode = FileTreeNode("ROOT", nullptr);
    std::vector<const FileTreeNode *> mVisibleNodes;
    bool mZoomChanged = false;
    float mUserZoom = 1.0f;

    void HandleInput();
    void HandleMenuBar();
    void HandleFileTree();
    void RenderFileTree();
    void HandlePropertiesWindow(std::shared_ptr<PCKAssetFile> &file);
    void RenderNode(FileTreeNode &node, std::vector<const FileTreeNode *> *visibleList = nullptr, bool shouldScroll = false, bool openFolder = false, bool closeFolder = false);
    void HandlePreviewWindow(const std::shared_ptr<PCKAssetFile> &file);
    void HandlePCKNodeContextMenu(const FileTreeNode &node);
    void ResetPreviewWindow(void);
    bool OpenPCKFile();

    void LoadPCKFile(const std::filesystem::path &filepath)
    {
        try
        {
            if (mCurrentPCKFile)
                mCurrentPCKFile->clearFiles();
            if (!mCurrentPCKFile)
                mCurrentPCKFile = std::make_shared<PCKFile>();
            mCurrentPCKFile->Read(filepath);
        }
        catch (...)
        {
            DBG_LOG("LoadPCKFile failed.");
        }
    }
};