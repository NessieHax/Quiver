<<<<<<< HEAD
#include "MenuFunctions.h"
#include "Application/Application.h"
#include <array>
#include <filesystem>

static SDL_DialogFileFilter pckFilter[] = { { "Minecraft LCE DLC Files (*.pck)", "pck"} };
=======
#include <sstream>
#include "Application/Application.h"
#include "Program/Program.h"
#include "UI/MenuFunctions.h"
#include "Util/Util.h"

static PlatformBase::FileDialogBase::FileFilter pckFilter[] = {
	{ "Minecraft LCE DLC Files (*.pck)", "pck" }
};
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

// TODO: move to header file
void ShowSuccessMessage()
{
<<<<<<< HEAD
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Success", "Operation performed successfully.", Application::Get().GetWindow());
=======
	if (inpath.empty())
		return;

	auto& fileDialog = gApp->GetPlatform()->mDialog;

	try {
		gApp->GetInstance()->LoadPCKFile(inpath);
	}
	catch (const std::exception& e) {
		fileDialog.ShowError("Error", e.what());
		return;
	}
	catch (...) {
		fileDialog.ShowError("Error", "Unknown Error Occurred.");
		return;
	}

	// if successful, reset node and UI data; pass file path to send to UI
	ResetProgramData();
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
}

void ShowCancelledMessage()
{
<<<<<<< HEAD
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Cancelled", "User aborted operation.", Application::Get().GetWindow());
}

std::filesystem::path OpenPCKFileDialog()
{
	std::string filePath = IO::OpenFileDialog(Application::Get().GetWindow(), pckFilter);
	return filePath;
}

void SavePCKFileDialog(std::shared_ptr<PCKFile> &pckFile, const std::string &defaultName)
{
	std::string filePath = IO::SaveFileDialog(Application::Get().GetWindow(), pckFilter, defaultName);
=======
	auto& fileDialog = gApp->GetPlatform()->mDialog;

	std::string filePath = fileDialog.OpenFile({ pckFilter[0] });

	if (!filePath.empty())
		OpenPCKFile(filePath);
	else
		fileDialog.ShowInfo("Cancelled", "User aborted operation.");
}

void SavePCKFileDialog(Binary::Endianness endianness, const std::string& defaultName)
{
	PCKFile* pckFile = gApp->GetInstance()->GetCurrentPCKFile();

	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;

	std::string filePath = fileDialog.SaveFile({ pckFilter[0] }, defaultName);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	if (!filePath.empty())
	{
		SavePCKFile(pckFile, filePath);
		pckFile->setFilePath(filePath); // update to save as location
	}
	else
		platform->ShowCancelledMessage();
}

<<<<<<< HEAD
void SavePCKFile(std::shared_ptr<PCKFile> &pckFile, const std::filesystem::path &outpath)
{
	try
	{
		pckFile->Write(outpath);
	}
	catch (const std::exception &e)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), Application::Get().GetWindow());
	}

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Saved", "File successfully saved!", Application::Get().GetWindow());
=======
void SavePCKFile(const std::string& outpath, Binary::Endianness endianness)
{
	PCKFile* pckFile = gApp->GetInstance()->GetCurrentPCKFile();

	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;

	try {
		pckFile->Write(outpath, endianness);
	}
	catch (const std::exception& e) {
		fileDialog.ShowError("Error", e.what());
		delete pckFile;
		pckFile = new PCKFile();
		return;
	}

	platform->ShowSuccessMessage();
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
}

void SetFilePropertiesDialog(PCKAssetFile &file)
{
<<<<<<< HEAD
	const static SDL_DialogFileFilter filters[] = {
		{"Text File", "txt"},
		{"All Files", "*"}};

	std::string inpath = IO::OpenFileDialog(Application::Get().GetWindow(), filters);
=======
	static PlatformBase::FileDialogBase::FileFilter filters[] = {
		{ "Text File", "txt" },
		{ "All Files", "*" }
	};

	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;
	std::string inpath = fileDialog.OpenFile({ filters[0], filters[1] });
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	if (!inpath.empty())
	{
		std::ifstream in(inpath, std::ios::binary);
		if (in)
		{
			file.clearProperties();
			Binary::TextEncoding encoding = Binary::DetectTextEncoding(in);

			std::string key;
			std::u16string value;

			if (encoding == Binary::TextEncoding::UTF8)
			{
				// Skip BOM if present
				char first3[3] = {0};
				in.read(first3, 3);
				if (!((unsigned char)first3[0] == 0xEF && (unsigned char)first3[1] == 0xBB && (unsigned char)first3[2] == 0xBF))
					in.seekg(0);

				std::string line;
				while (std::getline(in, line))
				{
					if (line.empty())
						continue;

					std::istringstream iss(line);
					if (!(iss >> key))
						continue;

					// Strip trailing colon, if any
					if (!key.empty() && key.back() == ':')
						key.pop_back();

					std::string value8;
					std::getline(iss, value8);
					value8.erase(0, value8.find_first_not_of(" \t"));

					value = Binary::ToUTF16(value8);

					file.addProperty(key, value);
				}
			}
			else if (encoding == Binary::TextEncoding::UTF16_LE || encoding == Binary::TextEncoding::UTF16_BE)
			{
				// Skip BOM (2 bytes)
				in.seekg(2);

				in.seekg(0, std::ios::end);
				size_t fileSize = in.tellg();
				in.seekg(2, std::ios::beg);

				size_t numChars = (fileSize - 2) / 2;
				std::vector<char16_t> buffer(numChars);

				in.read(reinterpret_cast<char *>(buffer.data()), numChars * 2);

				if (encoding == Binary::TextEncoding::UTF16_BE)
					Binary::SwapUTF16Bytes(buffer.data(), numChars);

				size_t start = 0;
				for (size_t i = 0; i <= numChars; ++i)
				{
					if (i == numChars || buffer[i] == u'\n')
					{
						std::u16string line(buffer.data() + start, i - start);
						start = i + 1;

						if (line.empty())
							continue;
						if (!line.empty() && line.back() == u'\r')
							line.pop_back();

						size_t key_end = 0;
						while (key_end < line.size() && line[key_end] != u' ' && line[key_end] != u'\t')
							++key_end;

						std::u16string key16 = line.substr(0, key_end);

						// Strip trailing colon, if any
						if (!key16.empty() && key16.back() == u':')
							key16.pop_back();
						if (key_end < line.size())
						{
							value = line.substr(key_end);

							// trimming the whitespace
							size_t pos = 0;
							while (pos < value.size() && (value[pos] == u' ' || value[pos] == u'\t'))
								++pos;
							if (pos > 0)
								value.erase(0, pos);
						}

						key = Binary::ToUTF8(key16);
					}
				}
			}
			if (key.empty()) // key cannot be empty; value can be empty, and is expected sometimes, like in the case of Texture ANIMs
			{
				file.addProperty(key, value);
			}
		}
	}
	else
		platform->ShowCancelledMessage();
}

<<<<<<< HEAD
SDL_DialogFileFilter GetFilter(const PCKAssetFile &file)
{
	std::array<const char *, 2> ext = file.getPreferredExtension();
=======
PlatformBase::FileDialogBase::FileFilter GetFilter(const PCKAssetFile& file)
{
	const std::vector<const char*> ext = file.getPreferredAssetExtension();
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	std::string nameStr;
	std::string patternStr;

	nameStr = std::string(file.getAssetTypeStringDisplay()) + " File | *." + ext[0];
	patternStr = ext[0];

	if (ext[1] != nullptr)
	{
		nameStr += ";*." + std::string(ext[1]);
		patternStr += ";" + std::string(ext[1]);
	}

	return { nameStr, patternStr };
}

<<<<<<< HEAD
bool SetDataFromFile(PCKAssetFile &file)
=======
bool SetFileDataDialog(PCKAssetFile& file)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	PlatformBase::FileDialogBase::FileFilter filters[] = {
		GetFilter(file),
		{"All Files", "*"}};

<<<<<<< HEAD
	std::string inpath = IO::OpenFileDialog(Application::Get().GetWindow(), filters);
=======
	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;
	std::string inpath = fileDialog.OpenFile({ filters[0], filters[1] });
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	if (!inpath.empty())
	{
		std::ifstream in(inpath, std::ios::binary | std::ios::ate);
		if (in)
		{
			std::streamsize size = in.tellg();
			in.seekg(0, std::ios::beg);
			std::vector<unsigned char> buffer(size);

			in.read(reinterpret_cast<char *>(buffer.data()), size);
			if (in.gcount() == size)
			{
				file.setData(buffer);
			}
			in.close();

			return true;
		}
	}

	platform->ShowCancelledMessage();
	return false;
}

<<<<<<< HEAD
void SaveFilePropertiesToFile(const PCKAssetFile &file, const std::filesystem::path &outpath)
=======
void WriteFileProperties(const PCKAssetFile& file, const std::string& outpath)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	if (outpath.empty())
		return;

	std::u16string propertyData;
<<<<<<< HEAD
	for (const auto &[key, val] : file.getProperties())
	{
		propertyData += IO::ToUTF16(key) + u' ' + val + u'\n';
=======
	for (const auto& [key, val] : file.getProperties()) {
		propertyData += Binary::ToUTF16(key) + u' ' + val + u'\n';
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
	}

	std::ofstream propFile(outpath, std::ios::binary);
	if (propFile.is_open())
	{
		const char *buffer = reinterpret_cast<const char *>(propertyData.data());
		std::size_t byteCount = propertyData.size() * sizeof(char16_t);
		propFile.write(buffer, byteCount);
		propFile.close();
	}
}

<<<<<<< HEAD
void SaveFilePropertiesDialog(const PCKAssetFile &file)
=======
void WriteFilePropertiesDialog(const PCKAssetFile& file)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	static const std::string nameStr = "Text File | *.txt";
	static const std::string patternStr = "*.txt";

	PlatformBase::FileDialogBase::FileFilter filter{ nameStr, patternStr };

<<<<<<< HEAD
	std::string outpath = IO::SaveFileDialog(Application::Get().GetWindow(), &filter, std::filesystem::path(file.getPath()).filename().string() + ".txt");
=======
	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;
	std::string outpath = fileDialog.SaveFile({ filter }, std::filesystem::path(file.getPath()).filename().string() + ".txt");
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	if (!outpath.empty())
	{
		WriteFileProperties(file, outpath);
		platform->ShowSuccessMessage();
	}
	else
		platform->ShowCancelledMessage();
}

<<<<<<< HEAD
void ExtractFileDataDialog(const PCKAssetFile &file, bool includeProperties)
=======
void WriteFileDataDialog(const PCKAssetFile& file, bool includeProperties)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	std::filesystem::path filePath(file.getPath());

	PlatformBase::FileDialogBase::FileFilter filter = GetFilter(file);

	const auto& platform = gApp->GetPlatform();
	auto& fileDialog = platform->mDialog;

<<<<<<< HEAD
	// this is very dumb and I'll have to give this a rewrite sometime
	if (includeProperties)
		outPath = IO::SaveFileDialogWithProperties(Application::Get().GetWindow(), &filter, file.getData(), filePath.filename().string(), true, file.getProperties());
	else
		outPath = IO::SaveFileDialogWithProperties(Application::Get().GetWindow(), &filter, file.getData(), filePath.filename().string());
=======
	std::string outPath = fileDialog.SaveFile({ filter }, filePath.filename().string());
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	if (!outPath.empty())
	{
		if(includeProperties)
			IO::WriteFile(outPath, file.getData(), file.getProperties());
		else
			IO::WriteFile(outPath, file.getData());

		platform->ShowSuccessMessage();
	}
	else
		platform->ShowCancelledMessage();
}
