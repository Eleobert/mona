#pragma once

#include "shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>



class text_renderer
{
    uint vao = 0;//, vbo = 0;
    struct character 
    {
        unsigned int TextureID;  // ID handle of the glyph texture
        glm::ivec2   Size;       // Size of glyph
        glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
        unsigned int Advance;    // Offset to advance to next glyph
    };

    struct vbos_t
    {
        uint pos = 0, tex = 0;
    } vbo;

    std::map<char, character> characters;
    
public:
    shader s;
    text_renderer(const std::string& font, int size);
    ~text_renderer();
    auto load_glyphs(FT_Library ft, FT_Face face) -> void;
    void render(std::string text, float x, float y, float scale = 1.f, glm::vec3 color = {0, 0, 0});
    void render(const std::string& text, glm::vec3 origin, glm::vec3 color);
    void render(std::string text, glm::vec3 origin, glm::vec3 dir, glm::vec3 color);

};