#include "slam/slam.h"

#include "mona/surface_mesh.hpp"

#include <cassert>

using namespace gl;


auto gen_indices(int n)
{
    assert(n > 0);
    std::vector<glm::uvec3> indices;
    indices.reserve(n * n);

    for (int j=0; j < n; j++)
    {
        for (int i=0; i < n; i++)
        {
            int row1 = (j + 0) * (n + 1);
            int row2 = (j + 1) * (n + 1);
            indices.push_back(glm::uvec3(row1+i, row1+i+1, row2+i+1));
            indices.push_back(glm::uvec3(row1+i, row2+i+1, row2+i));
        }
    }
    return indices;
}


auto gen_vertices(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z)
{
    std::vector<glm::vec3> vertices;

    vertices.reserve(x.size());

    auto x_span = std::abs(x.max() - x.min()) / 2.f;
    auto y_span = std::abs(y.max() - y.min()) / 2.f;
    auto scale  = std::max(x_span, y_span);

    for (auto i = 0; i < x.n_rows; i++)
    {
        for (auto j = 0; j < x.n_cols; j++)
        {
            float xi = x(i, j) / scale;
            float zi = y(i, j) / scale;
            float yi = z(i, j) / scale;

            vertices.push_back(glm::vec3(xi, yi, zi));
        }
    }
    return vertices;
}


auto gen_normals(const std::vector<glm::vec3>& vertices, const std::vector<glm::uvec3>& indices)
{
    auto normals = std::vector<glm::vec3>(vertices.size());

    for(auto i: indices)
    {
        auto a = vertices.at(i.x);
        auto b = vertices.at(i.y);
        auto c = vertices.at(i.z);

        normals[i.x] += glm::cross(b - a, c - a);
        normals[i.y] += glm::cross(a - b, c - b);
        normals[i.z] += glm::cross(a - c, b - c);
    }

    for(auto &n: normals)
    {
        n = glm::normalize(n);
    }

    return normals;
}

auto generate_vertex_data(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z)
    -> std::tuple<std::vector<glm::vec3>, std::vector<glm::vec3>, std::vector<glm::uvec3>>
 {
    assert(x.n_cols > 0);
     auto vertices = gen_vertices(x, y, z);
     // TODO: fix me
     assert(x.n_cols == x.n_rows);
     auto indices = gen_indices(x.n_cols - 1);
     auto normals = gen_normals(vertices, indices);

     return {vertices, normals, indices};
 }


 auto bind_buffer(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals,
                  const std::vector<glm::uvec3>& indices) -> uint
 {
     using namespace gl;

     GLuint  vao;
     glGenVertexArrays( 1, &vao );
     glBindVertexArray( vao );

     GLuint vbo;
     glGenBuffers( 1, &vbo );
     glBindBuffer( GL_ARRAY_BUFFER, vbo );
     glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW );
     glEnableVertexAttribArray( 0 );
     glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );


     GLuint vbo_normals;
     glGenBuffers( 1, &vbo_normals );
     glBindBuffer( GL_ARRAY_BUFFER, vbo_normals );
     glBufferData( GL_ARRAY_BUFFER, normals.size()*sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
     glEnableVertexAttribArray( 1 );
     glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

     GLuint ibo;
     glGenBuffers( 1, &ibo );
     glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibo );
     glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(glm::uvec3), indices.data(), GL_STATIC_DRAW );

     glBindVertexArray(0);
     glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
     glBindBuffer( GL_ARRAY_BUFFER, 0 );

     return vao;
 }

mona::surface_mesh::surface_mesh(const arma::fmat& x, const arma::fmat& y, const arma::fmat& z):
    s("../../../res/shaders/shader.vert", "../../../res/shaders/shader.frag")
{
    assert(x.n_rows == y.n_rows);
    assert(x.n_rows == z.n_rows);
    assert(x.n_cols == y.n_cols);
    assert(x.n_cols == z.n_cols);

    std::tie(vertices, normals, indices) = generate_vertex_data(x, y, z);

    vao = bind_buffer(vertices, normals, indices);
}


auto mona::surface_mesh::draw(const mona::camera& cam, mona::targets::target& t) const -> void
{
    t.begin_frame();
    s.use();
    s.set_uniform("mvp", cam.mvprsp());
    s.set_uniform("render_mode", 1);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    s.unuse();
    t.end_frame();

}


mona::surface_mesh::~surface_mesh()
{
    // TODO: do I need to delete the buffers
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
