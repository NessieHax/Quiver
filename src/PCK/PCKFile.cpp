<<<<<<< HEAD
#include "PCKFile.h"
#include "IO/BinaryReader.h"
#include "IO/BinaryWriter.h"
=======
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
#include <set>
#include "PCK/PCKFile.h"
#include "Binary/BinaryReader.h"
#include "Binary/BinaryWriter.h"

const char *XML_VERSION_STRING = "XMLVERSION"; // used for advanced/full box support for skins

void PCKFile::Read(const std::filesystem::path &inpath)
{
	BinaryReader reader(inpath);

	uint32_t version;
	reader.ReadData(&version, 4); // assume this is little endian

	uint32_t versionSwapped = Binary::SwapInt32(version); // swapped for endianness check; assume big endian

	if (versionSwapped >= 0 && versionSwapped <= 3)
	{
<<<<<<< HEAD
		mEndianess = IO::Endianness::BIG;
		DBG_LOG("Big Endian detected, version %u", versionSwapped);
=======
		mEndianess = Binary::Endianness::BIG;
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
		mVersion = versionSwapped;
		printf("Big Endian detected");
	}
	else if (version >= 0 && version <= 3)
	{
<<<<<<< HEAD
		mEndianess = IO::Endianness::LITTLE;
		DBG_LOG("Little Endian detected, version %u", version);
=======
		mEndianess = Binary::Endianness::LITTLE;
		printf("Little Endian detected");
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
		mVersion = version;
	}
	else
	{
		throw std::runtime_error("Invalid PCK version");
	}

	printf(", version %u\n", mVersion);

	reader.SetEndianness(mEndianess);

	uint32_t propertyCount = reader.ReadInt32();
<<<<<<< HEAD
	DBG_LOG("Properties: %u", propertyCount);
=======
	printf("Properties: %u\n", propertyCount);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	mProperties.clear();
	mProperties.reserve(propertyCount);

	for (int i = 0; i < propertyCount; i++)
	{
		uint32_t propertyIndex = reader.ReadInt32();
		uint32_t stringLength = reader.ReadInt32();

		std::string property = Binary::ToUTF8(reader.ReadU16String(stringLength));

<<<<<<< HEAD
		DBG_LOG("\tIndex: %u, Property: %s", propertyIndex, property.c_str());
=======
		printf("\tIndex: %u, Property: %s\n", propertyIndex, property.c_str());
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

		mProperties.push_back(property);

		reader.ReadInt32(); // skip 4 bytes
	}

	mXMLSupport = std::any_of(mProperties.begin(), mProperties.end(),
							  [](const std::string &property)
							  { return property == XML_VERSION_STRING; });

	if (mXMLSupport)
	{
		reader.ReadInt32(); // just "skip" 4 bytes
<<<<<<< HEAD
		DBG_LOG("XML Version: %u", mXMLSupport);
=======
		printf("XML Version: %u\n", mXMLSupport);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
	}

	uint32_t fileCount = reader.ReadInt32();
	mFiles.reserve(fileCount);
	for (uint32_t i{0}; i < fileCount; i++)
	{
		uint32_t fileSize = reader.ReadInt32();
		uint32_t fileType = reader.ReadInt32();
		uint32_t filePathLength = reader.ReadInt32();

		std::string filePath = Binary::ToUTF8(reader.ReadU16String(filePathLength));
		std::replace(filePath.begin(), filePath.end(), '\\', '/');

		reader.ReadInt32(); // skip 4 bytes

		mFiles.emplace_back(std::make_shared<PCKAssetFile>(filePath, PCKAssetFile::Type(fileType), fileSize));
	}

<<<<<<< HEAD
	DBG_LOG("Files: %u", fileCount);
=======
	printf("Files: %u\n", fileCount);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	for (int i = 0; i < mFiles.size(); ++i)
	{
		std::shared_ptr<PCKAssetFile> &file = mFiles[i];
		uint32_t propertyCount = reader.ReadInt32();

<<<<<<< HEAD
		DBG_LOG("\tSize: %zu Bytes | Type: %u | Properties: %u | Path: %ls", file->getFileSize(), (uint32_t)file->getAssetType(), propertyCount, file->getPath().c_str());
=======
		printf("\tSize: %u Bytes | Type: %u | Properties: %u | Path: %s\n", fileSizes[i], (uint32_t)file.getAssetType(), propertyCount, file.getPath().c_str());
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

		for (int j{0}; j < propertyCount; j++)
		{
			uint32_t propertyIndex = reader.ReadInt32();

			if (propertyIndex >= mProperties.size())
			{
				DBG_LOG("Property index out of range. skipping...");
				continue;
			}

			std::string propertyKey = mProperties[propertyIndex];
			uint32_t propertyValueLength = reader.ReadInt32();
			std::u16string propertyValue = reader.ReadU16String(propertyValueLength);

			reader.ReadInt32(); // skip 4 bytes

<<<<<<< HEAD
			DBG_LOG("\t\tProperty: %s %s", propertyKey.c_str(), IO::ToUTF8(propertyValue).c_str());
=======
			printf("\t\tProperty: %s %s\n", propertyKey.c_str(), Binary::ToUTF8(propertyValue).c_str());
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

			file->addProperty(propertyKey, propertyValue);
		}

		auto &buffer = file->getData();
		reader.ReadData(buffer.Data, buffer.Size);
	}

	setFilePath(inpath); // finally set the path if everything went well
}

<<<<<<< HEAD
void PCKFile::Write(const std::filesystem::path &outpath)
=======
void PCKFile::Write(const std::string& outpath, Binary::Endianness endianness)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	BinaryWriter writer(outpath);
	writer.SetEndianness(mEndianess);

	uint32_t versionOut = mVersion;
<<<<<<< HEAD
	if (mEndianess != IO::Endianness::LITTLE)
		versionOut = BinaryWriter::SwapInt32(mVersion);
=======
	if (endianness != Binary::Endianness::LITTLE)
		versionOut = Binary::SwapInt32(mVersion);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
	writer.WriteData(&versionOut, sizeof(uint32_t));

	// make new property list
	mProperties.clear();
	std::set<std::string> propertySet;

	if (mXMLSupport)
		mProperties.push_back(XML_VERSION_STRING);

	for (const auto &file : mFiles)
	{
		for (const auto &[key, _] : file->getProperties())
		{
			if (propertySet.insert(key).second) // only insert if not already in set
				mProperties.push_back(key);
		}
	}

	uint32_t propertyCount = static_cast<uint32_t>(mProperties.size());
	writer.WriteInt32(propertyCount);

	for (uint32_t i = 0; i < propertyCount; ++i)
	{
		writer.WriteInt32(i);
		writer.WriteInt32(static_cast<uint32_t>(mProperties[i].size()));
		writer.WriteU16String(Binary::ToUTF16(mProperties[i]));
		writer.WriteInt32(0); // skip 4 bytes
	}

	if (mXMLSupport)
	{
		writer.WriteInt32(3); // this is just for now until other XMLVersions are supported
	}

	uint32_t fileCount = static_cast<uint32_t>(mFiles.size());
	writer.WriteInt32(fileCount);

	for (const auto &file : mFiles)
	{
		writer.WriteInt32(static_cast<uint32_t>(file->getFileSize()));
		writer.WriteInt32(static_cast<uint32_t>(file->getAssetType()));

		const std::string &filePath = file->getPath().string();
		writer.WriteInt32(static_cast<uint32_t>(filePath.size()));
		writer.WriteU16String(Binary::ToUTF16(filePath));
		writer.WriteInt32(0); // skip 4 bytes
	}

	for (const auto &file : mFiles)
	{
		const auto &props = file->getProperties();
		writer.WriteInt32(static_cast<uint32_t>(props.size()));

		for (const auto &[key, value] : props)
		{
			auto it = std::find(mProperties.begin(), mProperties.end(), key);
			uint32_t index = static_cast<uint32_t>(std::distance(mProperties.begin(), it));
			writer.WriteInt32(index);
			writer.WriteInt32(static_cast<uint32_t>(value.size()));
			writer.WriteU16String(value);
			writer.WriteInt32(0); // skip 4 bytes
		}

		writer.WriteData(file->getData().Data, file->getFileSize());
	}
}

<<<<<<< HEAD
void PCKFile::addFile(const std::shared_ptr<PCKAssetFile> &file)
=======
void PCKFile::addFileFromDisk(const std::string& filepath, std::string new_filepath, PCKAssetFile::Type fileType)
{
	if (new_filepath.empty())
		new_filepath = std::filesystem::path(filepath).filename().string();

	std::ifstream in(filepath, std::ios::binary | std::ios::ate);
	if (!in.is_open())
		throw std::runtime_error("Could not open file: " + filepath);

	std::streamsize size = in.tellg();
	in.seekg(0, std::ios::beg);

	std::vector<unsigned char> buffer(size);
	if (!in.read(reinterpret_cast<char*>(buffer.data()), size))
		throw std::runtime_error("Failed to read file: " + filepath);

	PCKAssetFile asset(new_filepath, buffer, fileType);
	addFile(&asset);
}

void PCKFile::addFile(const PCKAssetFile* file)
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
{
	// emplace just sounds cooler, okay??
	mFiles.push_back(file);
}

void PCKFile::deleteFile(const std::shared_ptr<PCKAssetFile> &file)
{
	if (!file)
		return;

	auto it = std::remove_if(mFiles.begin(), mFiles.end(), [&](const std::shared_ptr<PCKAssetFile> &f)
							 { return f == file; });

	if (it != mFiles.end())
	{
		mFiles.erase(it, mFiles.end());
		return;
	}

	return;
}

void PCKFile::clearFiles()
{
	mFiles.clear();
}

int PCKFile::getFileIndex(const PCKAssetFile* file) const
{
	if (!file) return -1;

	for (size_t i = 0; i < mFiles.size(); ++i) {
		if (&mFiles[i] == file)
			return static_cast<int>(i);
	}
	return -1;
}

void PCKFile::moveFileToIndex(const PCKAssetFile* file, size_t newIndex)
{
	if (!file || newIndex >= mFiles.size())
		return;

	auto it = std::find_if(mFiles.begin(), mFiles.end(), [&](const PCKAssetFile& f) {
		return &f == file;
		});

	if (it == mFiles.end())
		return;

	PCKAssetFile temp = std::move(*it);
	mFiles.erase(it);

	auto insertIt = mFiles.begin() + newIndex;
	mFiles.insert(insertIt, std::move(temp));
}

uint32_t PCKFile::getPCKVersion() const
{
	return mVersion;
}

Binary::Endianness PCKFile::getEndianness() const
{
	return mEndianess;
}

const std::vector<std::string> &PCKFile::getPropertyKeys() const
{
	return mProperties;
}

const std::vector<std::shared_ptr<PCKAssetFile>> &PCKFile::getFiles() const
{
	return mFiles;
}

bool PCKFile::getXMLSupport() const
{
	return mXMLSupport;
}

void PCKFile::setXMLSupport(bool value)
{
	mXMLSupport = value;
}

PCKFile::~PCKFile()
{
	mProperties.clear();
	mFiles.clear();
}

const std::filesystem::path& PCKFile::getFilePath() const
{
	return mFilePath;
}

std::string PCKFile::getFileName() const
{
	return mFilePath.filename().string();
}

void PCKFile::setFilePath(const std::filesystem::path &pathin)
{
	mFilePath = pathin;
}