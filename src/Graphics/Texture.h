#pragma once
#include <glad/glad.h>
#include <filesystem>
#include <SDL3/SDL_log.h>

struct Texture
{
    GLuint id;
    int width;
    int height;

    Texture() : Texture(0, 0, -1) { }

    Texture(const Texture &&t) : Texture(t.width, t.height, t.id)
    {
        SDL_Log("Texture(const Texture &&t)");
    }

    Texture(Texture &&t) : Texture(t.width, t.height, t.id)
    {
        SDL_Log("Texture(Texture &&t)");
    }

    Texture(Texture &t) : Texture(t.width, t.height, t.id)
    {
        SDL_Log("Texture(Texture &t)");
    }

    Texture &Texture::operator =(const Texture &r)
    {
        SDL_Log("Texture::operator =(%d -> %d)", id, r.id);
        width = r.width;
        height = r.height;
        if (r.id != id)
            Release();
        id = r.id;
        return *this;
    }

    ~Texture()
    {
        SDL_Log("Texture::~Texture(%d)", id);
    }

    void Release()
    {
        if (id == -1)
            return;
        SDL_Log("Texture::Release(%d)", id);
        glDeleteTextures(1, &id);
        id = -1;
    }

    // Loads texture from memory; also takes optional GL Filter Parameter
    static bool LoadTextureFromMemory(const void *data, size_t size, Texture *result, int glFilter = GL_NEAREST);

    // Loads texture from file path; also takes optional GL Filter Parameter
    static bool LoadTextureFromFile(const std::filesystem::path &path, Texture *result, int glFilter = GL_NEAREST);

private:
    Texture(int width, int height, GLuint id) : width(width), height(height), id(id)
    {}
};