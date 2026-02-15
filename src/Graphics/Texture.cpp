#include "Texture.h"
#include <fstream>
#include "Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

bool Texture::LoadTextureFromMemory(const void* data, size_t size, Texture *result, int glFilter)
{
	DBG_LOG("%p s:%zd", data, size);
	if (!data || size == 0)
	{
		DBG_LOG("Empty buffer passed to LoadTextureFromMemory.");
		return false;
	}

	uint8_t *pixels = stbi_load_from_memory(
		reinterpret_cast<const unsigned char*>(data),
		static_cast<int>(size),
		&result->width,
		&result->height,
		NULL,
		4 // force RGBA
	);

	if (!pixels) {
		DBG_LOG("Failed to load image: %s", stbi_failure_reason());
		return false;
	}

	if (result->id == -1)
	{
		glGenTextures(1, &result->id);
		DBG_LOG("result->id=%d", result->id);
	}
	glBindTexture(GL_TEXTURE_2D, result->id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		result->width,
		result->height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels);

	// only generate textures with mipmaps enabled
	if (glFilter == GL_LINEAR_MIPMAP_LINEAR ||
		glFilter == GL_NEAREST_MIPMAP_LINEAR ||
		glFilter == GL_LINEAR_MIPMAP_NEAREST ||
		glFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(pixels);
	return true;
}

bool Texture::LoadTextureFromFile(const std::filesystem::path& path, Texture *result, int glFilter)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	if (!file)
	{
		DBG_LOG("Failed to open image file: %ls", path.c_str());
		return false;
	}

	std::size_t size = file.tellg();

	if (size <= 0) {
		DBG_LOG("Empty buffer passed to LoadTextureFromFile.");
		return false;
	}

	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> buffer(size);
	if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
	{
		DBG_LOG("Failed to read image file: %ls", path.c_str());
		return false;
	}

	DBG_LOG("Loaded: %ls", path.c_str());

	return LoadTextureFromMemory(buffer.data(), buffer.size(), result, glFilter);
}