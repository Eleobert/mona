#include "slam/slam.h"
#include "mona/text_renderer.hpp"

#include <iostream>
#include <vector>

auto init_freetype(const std::string& font, int size) -> std::tuple<FT_Library, FT_Face>
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::clog << "FreeType error: could not init library.\n";
        std::exit(-1);
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font\n";
        std::exit(-1);
    }

    FT_Set_Pixel_Sizes(face, 0, size);

    if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
        std::exit(-1);
    }
    return {ft, face};
}


text_renderer::text_renderer(const std::string& font, int size)
        : s("../../../res/shaders/text_shader.vert", "../../../res/shaders/text_shader.frag")
{

    using namespace gl;
    auto [ft, face] = init_freetype(font, size);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
    load_glyphs(ft, face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &vao);
    glGenBuffers(2, &vbo.pos);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo.pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    float tex_coord[6][2] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f },
                              { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }};

    glBindBuffer(GL_ARRAY_BUFFER, vbo.tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, tex_coord, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

text_renderer::~text_renderer()
{
    gl::glDeleteBuffers(2, &vbo.pos);
    gl::glDeleteVertexArrays(1, &vao);
}


auto text_renderer::load_glyphs(FT_Library ft, FT_Face face) -> void
{
    using namespace gl;

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        character ch = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        characters.insert(std::pair<char,character>(c, ch));
    }
}

void text_renderer::render(const std::string& text, glm::vec2  p, float scale, glm::vec3 color, glm::vec2 origin)
{
    using namespace gl;

    // activate corresponding render state
    s.use();
    s.set_uniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    p -= size(text, scale) * (origin + glm::vec2(1)) * 0.5f;

    // iterate through all characters

    for (const auto c: text)
    {
        character ch = characters[c];

        float xpos = p.x + ch.bearing.x * scale;
        float ypos = p.y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        // update VBO for each character
        float vertices[6][3] = {
                { xpos,     ypos + h,   0.0f },
                { xpos,     ypos,       0.0f },
                { xpos + w, ypos,       0.0f },

                { xpos,     ypos + h,   0.0f },
                { xpos + w, ypos,       0.0f },
                { xpos + w, ypos + h,   0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo.pos);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        p.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    s.unuse();
}


auto text_renderer::size(const std::string& text, float scale) -> glm::vec2
{
    auto w = 0.f;
    auto h = 0.f;
    for (const auto c: text)
    {
        character ch = characters[c];

        w += (ch.advance >> 6) * scale;
        h = std::max(h, static_cast<float>(ch.size.y));
    }

    return {w, h};
}