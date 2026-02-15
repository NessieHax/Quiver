#pragma once

<<<<<<< HEAD:src/IO/BinaryReader.h
#include <fstream>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <locale>
#include <codecvt>
#include "IO.h"
=======
#include "Binary/Binary.h"
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d:src/Binary/BinaryReader.h

// Barebones binary reader because it's nice I guess; inspired by miku666/NessieHax/nullptr's EndiannessAwareBinaryReader from the OMI/PCK Studio code <3
class BinaryReader
{
public:
	BinaryReader(const std::filesystem::path &filepath);
	~BinaryReader()
	{
		if (mStream)
			mStream.close();
	}

	// Sets endianness of the reader
	void SetEndianness(Binary::Endianness endianness);

	// Reads 8 bit unsigned int
	uint8_t ReadInt8();

	// Reads 16 bit unsigned int
	uint16_t ReadInt16();

	// Reads 32 bit unsigned int
	uint32_t ReadInt32();

	// Reads a U16 string by length
	std::u16string ReadU16String(size_t length);

<<<<<<< HEAD:src/IO/BinaryReader.h
	static uint16_t SwapInt16(uint16_t value);
	static uint32_t SwapInt32(uint32_t value);

	size_t ReadData(void *buffer, size_t size);
=======
	// Reads data into buffer of a given size
	void ReadData(void* buffer, size_t size);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d:src/Binary/BinaryReader.h

private:
	std::ifstream mStream;
	Binary::Endianness mEndianness = Binary::Endianness::LITTLE; // default to little since Little is used by more editions of the game
};