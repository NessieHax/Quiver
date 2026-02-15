#pragma once

<<<<<<< HEAD
#include <stdint.h>
#include <vector>
#include <memory>
#include <filesystem>
#include "PCKAssetFile.h"
#include "IO/IO.h"
#include "Log.h"
=======
#include <filesystem>
#include "Binary/Binary.h"
#include "PCK/PCKAssetFile.h"
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

// PCK File research done by Jam1Garner, Nobledez, NessieHax/Miku666/nullptr, myself (May/MattNL), and many others over the years.

class PCKFile
{
public:
	PCKFile() = default;
	~PCKFile();

	// Reads data into the PCK File from string; will add memory variant soon
	void Read(const std::filesystem::path &inpath);

	// Writes PCK File to a specifed location
<<<<<<< HEAD
	void Write(const std::filesystem::path &outpath);
=======
	void Write(const std::string& outpath, Binary::Endianness endianness);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	// Reads PCK Format/Version and sets Endianness
	uint32_t getPCKVersion() const;

	// Gets PCK File Endianness; Little Endian: Xbox One, PS4, PSVita, Nintendo Switch; Big Endian: Xbox 360, PS3, Wii U
	Binary::Endianness getEndianness() const;

	// Gets Registered Property Keys from the PCK File
	const std::vector<std::string> &getPropertyKeys() const;

	// Gets Files from the PCK File
	const std::vector<std::shared_ptr<PCKAssetFile>> &getFiles() const;

<<<<<<< HEAD
	void addFile(const std::shared_ptr<PCKAssetFile> &file);

	void deleteFile(const std::shared_ptr<PCKAssetFile> &file);
=======
	// Adds PCKAssetFile to the PCK file
	void addFile(const PCKAssetFile* file);

	// Adds PCKAssetFile to the PCK file from disk
	void addFileFromDisk(const std::string& filepath, std::string new_filepath, PCKAssetFile::Type fileType = PCKAssetFile::Type::TEXTURE);

	// Deletes PCKAssetFile from the PCK file
	void deleteFile(const PCKAssetFile* file);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

	// Clears the PCK File
	void clearFiles();

	// Gets the index of a given file
	int getFileIndex(const PCKAssetFile* file) const;

	// Moves a given file to a given index
	void moveFileToIndex(const PCKAssetFile* file, size_t newIndex);

	// Get XML Support. TODO: Replace with XMLVersion PROPERLY
	bool getXMLSupport() const;

	// Set XML Support. TODO: Replace with XMLVersion PROPERLY
	void setXMLSupport(bool value);

	// Get name of the file for display
	std::string getFileName() const;

<<<<<<< HEAD
	const std::filesystem::path& getFilePath() const;

	void setFilePath(const std::filesystem::path &pathin);

private:
	IO::Endianness mEndianess{IO::Endianness::LITTLE};
=======
	// Get the filepath
	std::string getFilePath() const;

	// Set the filepath
	void setFilePath(const std::string& pathin);

private:
	Binary::Endianness mEndianess{ Binary::Endianness::LITTLE };
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d
	bool mXMLSupport{false};
	uint32_t mVersion{};
	std::vector<std::string> mProperties{};
	std::vector<std::shared_ptr<PCKAssetFile>> mFiles{};
	std::filesystem::path mFilePath{};
};