#pragma once

#include "mona/internal/shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>



class text_renderer
{
    uint vao = 0;//, vbo = 0;
    struct character 
    {
        unsigned int texture_id; // ID handle of the glyph texture
        glm::vec2   size;        // Size of glyph
        glm::vec2   bearing;     // Offset from baseline to left/top of glyph
        long int advance;        // Offset to advance to next glyph
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
    void render(const std::string& text, glm::vec2 pos, float scale = 1.f,
                glm::vec3 color = {0, 0, 0}, glm::vec2 origin = {-1, 1});
    auto size(const std::string& text, float scale) -> glm::vec2;
};