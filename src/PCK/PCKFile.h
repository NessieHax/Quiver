#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <filesystem>
#include "PCKAssetFile.h"
#include "IO/IO.h"
#include "Log.h"

// PCK File research done by Jam1Garner, Nobledez, NessieHax/Miku666/nullptr, myself (May/MattNL), and many others over the years.

class PCKFile
{
public:
	PCKFile() = default;
	~PCKFile();

	// Reads data into the PCK File from string; will add memory variant soon
	void Read(const std::filesystem::path &inpath);

	// Writes PCK File to a specifed location
	void Write(const std::filesystem::path &outpath);

	// Reads PCK Format/Version and sets Endianness
	uint32_t getPCKVersion() const;

	// Gets PCK File Endianness; Little Endian: Xbox One, PS4, PSVita, Nintendo Switch; Big Endian: Xbox 360, PS3, Wii U
	IO::Endianness getEndianness() const;

	// Gets Registered Property Keys from the PCK File
	const std::vector<std::string> &getPropertyKeys() const;

	// Gets Files from the PCK File
	const std::vector<std::shared_ptr<PCKAssetFile>> &getFiles() const;

	void addFile(const std::shared_ptr<PCKAssetFile> &file);

	void deleteFile(const std::shared_ptr<PCKAssetFile> &file);

	void clearFiles();

	bool getXMLSupport() const;

	void setXMLSupport(bool value);

	std::string getFileName() const;

	const std::filesystem::path& getFilePath() const;

	void setFilePath(const std::filesystem::path &pathin);

private:
	IO::Endianness mEndianess{IO::Endianness::LITTLE};
	bool mXMLSupport{false};
	uint32_t mVersion{};
	std::vector<std::string> mProperties{};
	std::vector<std::shared_ptr<PCKAssetFile>> mFiles{};
	std::filesystem::path mFilePath{};
};