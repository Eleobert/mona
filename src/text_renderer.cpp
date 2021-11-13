#include "slam/slam.h"
#include "text_renderer.hpp"

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
        : s("../../res/shaders/text_shader.vert", "../../res/shaders/text_shader.frag")
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 3, NULL, GL_DYNAMIC_DRAW);
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

void text_renderer::render(std::string text, float x, float y, float scale, glm::vec3 color)
{
    using namespace gl;

    // activate corresponding render state
    s.use();
    s.set_uniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        character ch = characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
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
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo.pos);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void text_renderer::render(const std::string& text, glm::vec3 origin, glm::vec3 color)
{
    using namespace gl;

    // activate corresponding render state
    s.use();
    s.set_uniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        character ch = characters[*c];

        float xpos = origin.x + ch.Bearing.x;
        float ypos = origin.y - (ch.Size.y - ch.Bearing.y);

        float w = ch.Size.x;
        float h = ch.Size.y;
        // update VBO for each character
        std::vector<glm::vec3> vertices = {
                { xpos,     ypos + h, origin.z },
                { xpos,     ypos,     origin.z },
                { xpos + w, ypos,     origin.z },

                { xpos,     ypos + h, origin.z },
                { xpos + w, ypos,     origin.z },
                { xpos + w, ypos + h, origin.z }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo.pos);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        origin.x += (ch.Advance >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void text_renderer::render(std::string text, glm::vec3 origin, glm::vec3 dir, glm::vec3 color)
{
    using namespace gl;

    // activate corresponding render state
    s.use();
    s.set_uniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);
    auto scale = 0.01;
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        character ch = characters[*c];

        float xpos = origin.x + ch.Bearing.x * dir.x;
        float ypos = origin.y - (ch.Size.y - ch.Bearing.y) * dir.y;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][3] = {
                { xpos,     ypos + h,   origin.z },
                { xpos,     ypos,       origin.z },
                { xpos + w, ypos,       origin.z },

                { xpos,     ypos + h,   origin.z },
                { xpos + w, ypos,       origin.z },
                { xpos + w, ypos + h,   origin.z }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo.pos);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        origin.x += (ch.Advance >> 6) * dir.x; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

