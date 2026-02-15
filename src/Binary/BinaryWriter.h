#pragma once

<<<<<<< HEAD:src/IO/BinaryWriter.h
#include <fstream>
#include <filesystem>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include "IO.h"
=======
#include "Binary/Binary.h"
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d:src/Binary/BinaryWriter.h

// Barebones binary writer because it's nice I guess; inspired by miku666/NessieHax/nullptr's EndiannessAwareBinaryWriter from the OMI/PCK Studio code <3
class BinaryWriter
{
public:
	BinaryWriter(const std::filesystem::path& filepath);
	~BinaryWriter()
	{
		if (mStream)
			mStream.close();
	}

	// Sets endianness of the writer
	const void SetEndianness(Binary::Endianness endianness);

	// Writes 8 bit unsigned int
	void WriteInt8(const uint8_t value);

	// Writes 16 bit unsigned int
	void WriteInt16(const uint16_t value);

	// Writes 32 bit unsigned int
	void WriteInt32(const uint32_t value);

	// Writes U16 string of length
	void WriteU16String(const std::u16string& utf16str);

	// Wries data from buffer of a given size
	const void WriteData(const void* buffer, size_t size);

private:
	std::ofstream mStream;
	Binary::Endianness mEndianness = Binary::Endianness::LITTLE; // default to little since Little is used by more editions of the game
};