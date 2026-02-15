#include "UILayer.h"
#include "imgui_internal.h"

void UILayer::OnAttach(void)
{
	std::filesystem::path path(L"assets");
	path /= L"icons";
	Texture::LoadTextureFromFile(path / L"NODE_FOLDER.png", &mFolderIcon, GL_LINEAR_MIPMAP_LINEAR);

	for (int i = 0; i < (int)PCKAssetFile::Type::PCK_ASSET_TYPES_TOTAL; i++)
	{
		auto type = static_cast<PCKAssetFile::Type>(i);
		
		std::string name = (type == PCKAssetFile::Type::UI_DATA)
							   ? PCKAssetFile::getAssetTypeString(PCKAssetFile::Type::PCK_ASSET_TYPES_TOTAL)
							   : PCKAssetFile::getAssetTypeString(type);
		Texture::LoadTextureFromFile(path / (L"FILE_" + std::wstring(name.begin(), name.end()) + L".png"), &mFileIcons[i], GL_LINEAR_MIPMAP_LINEAR);
	}

	ImGuiStyle &style = ImGui::GetStyle();
	style.CellPadding = ImVec2(0, 0);

	ImFontConfig config;
	config.MergeMode = false;
	config.PixelSnapH = true;

	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/m6x11plus.ttf", 18.0f, &config);

	config.MergeMode = true;

	// Merge Chinese (Simplified)
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/ark-pixel-12px-monospaced-zh_cn.ttf", 18.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
	// Merge Chinese (Traditional)
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/ark-pixel-12px-monospaced-zh_tw.ttf", 18.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());
	// Merge Japanese
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/ark-pixel-12px-monospaced-ja.ttf", 18.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
	// Merge Korean
	ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/ark-pixel-12px-monospaced-ko.ttf", 18.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesKorean());

	ImGui::GetIO().Fonts->Build();
}

void UILayer::OnDetach(void)
{
	mSelectedNodePath = "";
	mVisibleNodes.clear();
	mRootNode.children.clear();
	mPreviewTexture.Release();
	mFolderIcon.Release();
	for (Texture &tex : mFileIcons)
		tex.Release();
	mFileIcons.clear();
}

void UILayer::OnUpdate(void)
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::Begin("aa", nullptr, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);
	HandleInput();
	HandleMenuBar();
	HandleFileTree();
	ImGui::End();
	ImGui::PopStyleVar();
}

void UILayer::HandleMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "Ctrl+O"))
			{
				OpenPCKFile();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, true))
			{
				SavePCK(mCurrentPCKFile, mRootNode, mCurrentPCKFile->getFilePath(), "");
			}
			if (ImGui::MenuItem("Save as", "Ctrl+Shift+S", nullptr, true))
			{
				SavePCK(mCurrentPCKFile, mRootNode, "", mCurrentPCKFile->getFileName());
			}
			ImGui::EndMenu();
		}

		if (mCurrentPCKFile)
		{
			IO::Endianness Endianness = mCurrentPCKFile->getEndianness();
			if (ImGui::BeginMenu("PCK"))
			{
				ImGui::Text("PCK Format:");
				if (ImGui::RadioButton("Little Endian (Xbox One, PS4, PSVita, Nintendo Switch)", Endianness == IO::Endianness::LITTLE))
				{
					Endianness = IO::Endianness::LITTLE;
				}

				if (ImGui::RadioButton("Big Endian (Xbox 360, PS3, Wii U)", Endianness == IO::Endianness::BIG))
				{
					Endianness = IO::Endianness::BIG;
				}

				ImGui::NewLine();
				bool hasXMLVerion = mCurrentPCKFile->getXMLSupport();
				if (ImGui::Checkbox("Full BOX Support (for Skins)", &hasXMLVerion))
				{
					mCurrentPCKFile->setXMLSupport(hasXMLVerion);
				}

				ImGui::EndMenu();
			}
		}
		ImGui::EndMainMenuBar();
	}
}

bool UILayer::OpenPCKFile()
{
	const std::filesystem::path &inpath = OpenPCKFileDialog();
	if (inpath.empty())
		return false;

	try {
		LoadPCKFile(inpath);
		BuildFileTree(mCurrentPCKFile->getFiles(), mRootNode);
		return true;
	}
	catch (const std::exception& e) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), Application::Get().GetWindow());
		return false;
	}
	catch (...) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Unknown Error Occurred.", Application::Get().GetWindow());
		return false;
	}

	// if successful, reset node and UI data; pass file path to send to UI
	// ResetUIData(inpath);
	return false;
}

void UILayer::HandleInput()
{
	// make sure to pass false or else it will trigger multiple times
	if (ImGui::IsKeyPressed(ImGuiKey_Delete, false))
	{
		if (ShowYesNoMessagePrompt("Are you sure?", "This is permanent and cannot be undone.\nIf this is a folder, all sub-files will be deleted too."))
		{
			if (FileTreeNode *node = FindNodeByPath(mSelectedNodePath, mRootNode.children))
			{
				DeleteNode(*node, mRootNode.children);
				TreeToPCKFileCollection(mCurrentPCKFile, mRootNode);
			}
		}
		else
			ShowCancelledMessage();
	}

	if (ImGui::GetIO().KeyCtrl)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_O, false))
		{
			OpenPCKFile();
		}
		else if (ImGui::GetIO().KeyShift && ImGui::IsKeyPressed(ImGuiKey_S, false))
		{
			SavePCK(mCurrentPCKFile, mRootNode, mCurrentPCKFile->getFilePath(), ""); // Save
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_S, false))
		{
			SavePCK(mCurrentPCKFile, mRootNode, "", mCurrentPCKFile->getFileName()); // Save As
		}
	}
}

static int ShowMessagePrompt(const char *title, const char *message, const SDL_MessageBoxButtonData *buttons, int numButtons)
{
	static SDL_MessageBoxData messageboxdata = {};
	messageboxdata.flags = SDL_MESSAGEBOX_WARNING | SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
	messageboxdata.title = title;
	messageboxdata.message = message;
	messageboxdata.numbuttons = numButtons;
	messageboxdata.buttons = buttons;
	messageboxdata.window = NULL;

	int buttonID = -1;
	if (SDL_ShowMessageBox(&messageboxdata, &buttonID))
	{
		return buttonID; // return the button ID user clicked
	}
	else
	{
		SDL_Log("Failed to show message box: %s", SDL_GetError());
		return -1; // indicate error
	}
}

static bool ShowYesNoMessagePrompt(const char *title, const char *message)
{
	const static SDL_MessageBoxButtonData buttons[] = {
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes"},
		{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "No"}};

	return ShowMessagePrompt(title, message, buttons, SDL_arraysize(buttons)) == 1;
}

void UILayer::ResetPreviewWindow(void)
{
	mPreviewTexture = {};
	mLastPreviewedFile = nullptr;
}

// Renders and handles window to preview the currently selected file if any data is previewable
void UILayer::HandlePreviewWindow(const std::shared_ptr<PCKAssetFile> &file)
{
	// if ID is valid AND last file is not the current file
	if (mPreviewTexture.id != 0 && mLastPreviewedFile != file.get())
	{
		mZoomChanged = false;
		mUserZoom = 1.0f;
	}

	if (mLastPreviewedFile != file.get())
	{
		if (!Texture::LoadTextureFromMemory(file->getData().Data, file->getFileSize(), &mPreviewTexture))
		{
			SDL_Log("Failed to load Preview Texture.");
		}
		mLastPreviewedFile = file.get();
		mPreviewTitle = file->getPath().string() + " (" + std::to_string(mPreviewTexture.width) + "x" + std::to_string(mPreviewTexture.height) + ")###Preview";

		mUserZoom = 1.0f;
		mZoomChanged = false;
	}
	if (mPreviewTexture.id == 0)
		return;

	float previewPosX = ImGui::GetIO().DisplaySize.x * 0.25f;
	ImVec2 previewWindowSize(ImGui::GetIO().DisplaySize.x * 0.75f, ImGui::GetIO().DisplaySize.y - (ImGui::GetIO().DisplaySize.y * 0.35f));
	// ImGui::SetNextWindowPos(ImVec2(previewPosX, ImGui::GetFrameHeight()), ImGuiCond_Always);
	// ImGui::SetNextWindowSize(previewWindowSize, ImGuiCond_Always);

	ImGui::Begin(mPreviewTitle.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::BeginChild("PreviewScroll", ImVec2(0, 0), false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::IsWindowHovered() && ImGui::GetIO().KeyCtrl && ImGui::GetIO().MouseWheel != 0.0f)
	{
		float zoomDelta = ImGui::GetIO().MouseWheel * 0.1f;
		mUserZoom = std::clamp(mUserZoom * (1.0f + zoomDelta), 0.01f, 100.0f); // this clamp is a little weird but it works lol
		mZoomChanged = true;
	}

	ImVec2 availSize = ImGui::GetContentRegionAvail();

	if (!mZoomChanged && mUserZoom == 1.0f)
	{
		mUserZoom = std::min(
			(availSize.x) / mPreviewTexture.width,
			(availSize.y) / mPreviewTexture.height);
		SDL_Log("%.2f", mUserZoom);
	}

	ImVec2 imageSize = ImVec2(mPreviewTexture.width * mUserZoom, mPreviewTexture.height * mUserZoom);

	ImVec2 cursorPos = ImGui::GetCursorPos();
	if (imageSize.x < availSize.x)
		cursorPos.x += (availSize.x - imageSize.x) / 2.0f;
	if (imageSize.y < availSize.y)
		cursorPos.y += (availSize.y - imageSize.y) / 2.0f;
	ImGui::SetCursorPos(cursorPos);
	ImGui::Image((ImTextureID)(intptr_t)mPreviewTexture.id, imageSize);

	std::stringstream ss;
	ss << "Zoom: " << std::fixed << std::setprecision(2) << (mUserZoom * 100.0f) << "%";
	std::string zoomText = ss.str();
	ImVec2 textSize = ImGui::CalcTextSize(zoomText.c_str());
	ImVec2 textPos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - textSize.x - 20, ImGui::GetWindowPos().y);
	ImGui::SetCursorScreenPos(textPos);
	ImGui::TextUnformatted(zoomText.c_str());

	ImGui::EndChild();
	ImGui::End();
}

static void HandlePropertiesContextWindow(std::shared_ptr<PCKAssetFile> &file, int propertyIndex = -1)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Add"))
		{
			file->addProperty("KEY", u"VALUE");
		}
		if (propertyIndex > -1 && ImGui::MenuItem("Delete"))
		{
			file->removeProperty(propertyIndex);
		}
		ImGui::EndPopup();
	}
}

void UILayer::HandlePropertiesWindow(std::shared_ptr<PCKAssetFile> &file)
{
	if (mLastPreviewedFile != file.get())
	{
		mLastPreviewedFile = file.get();
	}

	const auto properties = file->getProperties();

	ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::BeginPopupContextWindow("PropertiesContextWindow"))
	{
		if (ImGui::MenuItem("Add"))
			file->addProperty("KEY", u"VALUE");
		ImGui::EndPopup();
	}

	if (properties.empty())
	{
		ImGui::Text("NO PROPERTIES");
	}
	else
	{
		int propertyIndex = 0;

		if (ImGui::BeginTable("PropertiesTable", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV))
		{
			float maxKeyWidth = ImGui::CalcTextSize("KEY").x;
			for (const auto &[key, _] : properties)
			{
				ImVec2 size = ImGui::CalcTextSize(key.c_str());
				if (size.x > maxKeyWidth)
					maxKeyWidth = size.x;
			}
			maxKeyWidth += 10.0f;

			ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, maxKeyWidth);
			ImGui::TableSetupColumn("Value");
			ImGui::TableHeadersRow();

			int propertyIndex = 0;
			for (const auto &[key, value] : properties)
			{
				ImGui::TableNextRow();

				char keyBuffer[0x11];
				char valueBuffer[0x1001];

				std::strncpy(keyBuffer, key.c_str(), sizeof(keyBuffer) - 1);
				keyBuffer[sizeof(keyBuffer) - 1] = '\0';

				std::string utf8Value = IO::ToUTF8(value);
				std::size_t len = std::min(utf8Value.size(), sizeof(valueBuffer) - 1);
				std::memcpy(valueBuffer, utf8Value.data(), len);
				valueBuffer[len] = '\0';

				bool modified = false;

				ImGui::TableSetColumnIndex(0);
				std::string keyLabel = "##Key" + std::to_string(propertyIndex);
				ImGui::SetNextItemWidth(-FLT_MIN); // this is needed to make the input the full size of the column for some reason
				if (ImGui::InputText(keyLabel.c_str(), keyBuffer, sizeof(keyBuffer)))
					modified = true;

				// context menu
				HandlePropertiesContextWindow(file, propertyIndex);

				ImGui::TableSetColumnIndex(1);
				std::string valueLabel = "##Value" + std::to_string(propertyIndex);
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (ImGui::InputText(valueLabel.c_str(), valueBuffer, sizeof(valueBuffer)))
					modified = true;

				// context menu; again because I want it to work with both rows
				HandlePropertiesContextWindow(file, propertyIndex);

				if (modified)
				{
					std::string keyText = keyBuffer;
					for (char &c : keyText)
						c = std::toupper(c);

					file->setPropertyAtIndex(propertyIndex, keyText.empty() ? "KEY" : keyText, IO::ToUTF16(valueBuffer));
				}

				++propertyIndex;
			}

			ImGui::EndTable();
		}
	}

	ImGui::End();
}

void UILayer::HandlePCKNodeContextMenu(const FileTreeNode &node)
{
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::BeginMenu("Extract"))
		{
			if (node.file && ImGui::MenuItem("File"))
			{
				ExtractFileDataDialog(*node.file);
			}
			if (!node.file && ImGui::MenuItem("Files"))
			{
				SaveFolderAsFiles(node);
			}
			if (!node.file && ImGui::MenuItem("Files with Properties"))
			{
				SaveFolderAsFiles(node, true);
			}

			bool hasProperties = node.file && !node.file->getProperties().empty();

			if (node.file && hasProperties && ImGui::MenuItem("Properties"))
			{
				SaveFilePropertiesDialog(*node.file);
			}

			if (node.file && hasProperties && ImGui::MenuItem("File with Properties"))
			{
				ExtractFileDataDialog(*node.file, true);
			}

			ImGui::EndMenu();
		}
		if (node.file && ImGui::BeginMenu("Replace"))
		{
			if (ImGui::MenuItem("File Data"))
			{
				if (SetDataFromFile(*node.file))
					ResetPreviewWindow();
			}
			if (ImGui::MenuItem("File Properties"))
			{
				SetFilePropertiesDialog(*node.file);
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Delete"))
		{
			if (ShowYesNoMessagePrompt("Are you sure?", "This is permanent and cannot be undone.\nIf this is a folder, all sub-files will be deleted too."))
			{
				DeleteNode(node, mRootNode.children);
				TreeToPCKFileCollection(mCurrentPCKFile, mRootNode);
			}
			else
				ShowCancelledMessage();
		}
		ImGui::EndPopup();
	}
}

bool IsClicked()
{
	return (ImGui::IsItemClicked(ImGuiMouseButton_Left) || ImGui::IsItemClicked(ImGuiMouseButton_Right)) ||
		   // for context support; selecting and opening a node when a context menu is already opened
		   (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && ImGui::IsItemHovered());
}

// Renders the passed node, also handles children or the node
void UILayer::RenderNode(FileTreeNode &node, std::vector<const FileTreeNode *> *visibleList, bool shouldScroll, bool openFolder, bool closeFolder)
{
	if (visibleList)
		visibleList->push_back(&node); // adds node to visible nodes vector, but only when needed

	bool isFolder = (node.file == nullptr);
	bool isSelected = (node.path == mSelectedNodePath);

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	if (isSelected)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		ScrollToNode(shouldScroll);
	}

	if (isFolder)
	{
		ImGui::PushID(node.path.c_str());
		ImGui::Image((void *)(intptr_t)mFolderIcon.id, ImVec2(48, 48));
		ImGui::SameLine();

		if (node.path == mSelectedNodePath && (openFolder || closeFolder))
			ImGui::SetNextItemOpen(openFolder, ImGuiCond_Always);

		float avail = ImGui::GetContentRegionAvail().y;
		float off = (48/2-ImGui::GetTextLineHeight()/2);
		if (off > 0.0f)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
		bool open = ImGui::TreeNodeEx(node.path.c_str(), flags);

		if (IsClicked())
			mSelectedNodePath = node.path;

		HandlePCKNodeContextMenu(node);

		if (open)
		{
			for (auto &child : node.children)
				RenderNode(child, visibleList, shouldScroll, openFolder, closeFolder);
			ImGui::TreePop();
		}
		ImGui::PopID();
	}
	else
	{
		const PCKAssetFile &file = *node.file;
		ImGui::Image(ImTextureRef(mFileIcons[(int)file.getAssetType()].id), ImVec2(48, 48));
		ImGui::SameLine();
		ImGuiStyle& style = ImGui::GetStyle();
		float avail = ImGui::GetContentRegionAvail().y;
		float off = (48/2-ImGui::GetTextLineHeight()/2);
		if (off > 0.0f)
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + off);
		if (ImGui::Selectable((file.getPath().filename().string() + "###" + file.getPath().string()).c_str(), isSelected, 0))
			mSelectedNodePath = node.path;

		if (IsClicked())
			mSelectedNodePath = node.path;

		HandlePCKNodeContextMenu(node);
	}
}

// Renders the file tree... duh
void UILayer::RenderFileTree()
{
	bool shouldScroll = false;

	mVisibleNodes.clear();
	ImGuiWindowClass window_class;
	window_class.DockingAlwaysTabBar = false;

	ImGui::Begin(std::string(mCurrentPCKFile->getFileName()).c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

	bool shouldOpenFolder = false;
	bool shouldCloseFolder = false;

	if (ImGui::IsWindowFocused() && !mSelectedNodePath.empty())
	{
		shouldOpenFolder = ImGui::IsKeyPressed(ImGuiKey_RightArrow);
		shouldCloseFolder = !shouldOpenFolder && ImGui::IsKeyPressed(ImGuiKey_LeftArrow);
	}

	for (auto &node : mRootNode.children)
		RenderNode(node, &mVisibleNodes, shouldScroll, shouldOpenFolder, shouldCloseFolder);

	static int selectedIndex = -1;
	if (ImGui::IsWindowFocused() && !mVisibleNodes.empty())
	{
		for (int i = 0; i < (int)mVisibleNodes.size(); ++i)
		{
			if (mVisibleNodes[i]->path == mSelectedNodePath)
			{
				selectedIndex = i;
				break;
			}
		}

		std::string previousPath = mSelectedNodePath;
		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
		{
			selectedIndex = std::max(0, selectedIndex - 1);
			mSelectedNodePath = mVisibleNodes[selectedIndex]->path;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
		{
			selectedIndex = std::min((int)mVisibleNodes.size() - 1, selectedIndex + 1);
			mSelectedNodePath = mVisibleNodes[selectedIndex]->path;
		}
		if (mSelectedNodePath != previousPath)
			shouldScroll = true;
	}

	std::shared_ptr<PCKAssetFile> selectedFile = nullptr;

	FileTreeNode *selectedNode = FindNodeByPath(mSelectedNodePath, mRootNode.children);
	if (selectedNode && selectedNode->file)
		selectedFile = selectedNode->file;

	if (selectedFile)
	{
		if (selectedFile->isImageType())
		{
			HandlePreviewWindow(selectedFile);
		}

		HandlePropertiesWindow(selectedFile);
	}

	shouldOpenFolder = false;
	shouldCloseFolder = false;
	ImGui::End();
}

void UILayer::HandleFileTree()
{
	if (mCurrentPCKFile)
	{
		RenderFileTree();
	}
}