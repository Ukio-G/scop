#ifndef MATH_42_HPP
#define MATH_42_HPP

#include <cmath>
#include <cstddef>
#include <optional>
#include <ostream>
#include <vector>

namespace glm42 {
    static constexpr double pi = 3.14159265;

    template<typename T, size_t dim>
    struct vec {
    public:
        vec(const vec &other) {
            for (int i = 0; i < dim; i++) {
                data[i] = other.data[i];
            }
        }

        explicit vec(const vec<T, dim - 1> & other) {
          for (int i = 0; i < dim - 1; i++) {
            data[i] = other.data[i];
          }
          data[dim - 1] = 0;
        }

        explicit vec(T item) {
            for (int i = 0; i < dim; i++) {
                data[i] = item;
            }
        }

        vec(const vec<T, dim - 1> &other, T last_item) {
            for (int i = 0; i < dim - 1; i++) {
                data[i] = other.data[i];
            }
            data[dim] = last_item;
        }


        vec() {
            for (int i = 0; i < dim; i++) {
                data[i] = static_cast<T>(0);
            }
        }

        T operator[](int i) const {
            return data[i];
        }


        T &operator[](int i) {
            return data[i];
        }

        bool operator==(const vec<T, dim>& other) {
          for (int i = 0; i < dim; ++i) {
            if (data[i] != other.data[i])
              return false;
          }
          return true;
        }

        T data[dim];
    };

    class vec3 : public vec<double, 3> {
    public:
        vec3() : vec(0) {}

        vec3(const vec<double, 3>& other ) : vec(other) {}

        explicit vec3(double i) : vec(i) {
        }

        vec3(const vec3 &other) = default;

        vec3(double x, double y, double z) : vec(0.0) {
            data[0] = x;
            data[1] = y;
            data[2] = z;
        }
    };

    inline vec3 operator*(vec3 lhs, double rhs) {
      lhs[0] *= rhs;
      lhs[1] *= rhs;
      lhs[2] *= rhs;
      return lhs;
    }

    class vec4 : public vec<double, 4> {
    public:
        vec4() : vec(0) {}
        explicit vec4(double i) : vec(i) {
        }

        vec4(const vec4 &other) = default;

        explicit vec4(const vec3 &other) : vec(other) {
            data[3] = 0;
        }

        vec4(double x, double y, double z, double w) : vec(0.0) {
            data[0] = x;
            data[1] = y;
            data[2] = z;
            data[3] = w;
        }
    };

    class vec2 : public vec<double, 2> {
    public:
        vec2() : vec(0) {}

        explicit vec2(double i) : vec(i) {
        }

        vec2(const vec2 &other) = default;

        vec2(double x, double y) : vec(0.0) {
            data[0] = x;
            data[1] = y;
        }
    };

    template<class T, size_t dim>
    vec<T, dim> operator+(vec<T, dim> lhs, vec<T, dim> rhs) {
      for (int i = 0; i < dim; i++)
        lhs[i] += rhs[i];
      return lhs;
    }

    template<class T, size_t dim>
    vec<T, dim> operator-(vec<T, dim> lhs, vec<T, dim> rhs) {
      for (int i = 0; i < dim; i++)
        lhs[i] -= rhs[i];
      return lhs;
    }

    template<typename T, size_t dim>
    struct mat {
        using vec_type = vec<T, dim>;

        mat(const mat &other) {
            for (int col = 0; col < dim; col++) {
                for (int row = 0; row < dim; row++) {
                    data[col][row] = other.data[col][row];
                }
            }
        }

        explicit mat(T item) {
            for (int col = 0; col < dim; col++) {
                for (int row = 0; row < dim; row++) {
                    data[col][row] = item;
                }
            }
        }

        mat<T, dim> &operator=(const mat &other) {
            if (this == &other)
                return *this;

            for (int col = 0; col < dim; col++) {
                for (int row = 0; row < dim; row++) {
                    data[col][row] = other.data[col][row];
                }
            }

            return *this;
        }

        mat() {
            *this = mat(static_cast<T>(0));
        }


        T& at(int col, int row) {
            return data[col][row];
        }

        mat<T, dim> operator*(const mat<T, dim> &other) const {
            mat<T, dim> result;
            for (int row = 0; row < dim; row++) {
                for (int col = 0; col < dim; col++) {
                    for (int k = 0; k < dim; k++) {
                        result.data[col][row] += data[col][k] * other.data[k][row];
                    }
                }
            }

            return result;
        }

        vec<T, dim> operator*(const vec<T, dim> &other) const {
            vec<T, dim> result(0);
            for (int row = 0; row < dim; row++) {
                for (int k = 0; k < dim; k++) {
                    result.data[row] += data[k][row] * other.data[k];
                }
            }

            return result;
        }

        mat<T, dim> operator*(const T number) const {
            mat<T, dim> result;
            for (int col = 0; col < dim; col++) {
                for (int row = 0; row < dim; row++) {
                    result.data[col][row] = data[col][row] * number;
                }
            }

            return result;
        }

        mat<T, dim> operator+(const mat<T, dim> &other) const {
            mat<T, dim> result;
            for (int col = 0; col < dim; col++) {
                for (int row = 0; row < dim; row++) {
                    result.data[col][row] = data[col][row] + other.data[col][row];
                }
            }

            return result;
        }


        static mat<T, dim> diag(T item) {
            mat<T, dim> result;
            for (int i = 0; i < dim; ++i) {
                result.data[i][i] = item;
            }
            return result;
        };

        static mat<T, dim> id() {
            return diag(static_cast<T>(1));
        };

        T data[dim][dim];
    };


    // No, GLM is column major. Always has been.
    /*

                    m[0]    m[1]   m[2]    m[3]
        m[0][0]     0       4      8       12
        m[0][1]     1       5      9       13
        m[0][2]     2       6      10      14
        m[0][3]     3       7      11      15

     */
    template<typename T, size_t dim>
    inline mat<T, dim> translate(const mat<T, dim> &m, vec<T, dim> v) {

    }

    template<typename T, size_t dim>
    inline mat<T, dim> scale(const mat<T, dim> &m, vec<T, dim> v) {

    }

    template<typename T, size_t dim>
    inline mat<T, dim> rotate(const mat<T, dim> &m, double radians, const vec<T, dim> &v) {

    }

    template<typename T, size_t dim>
    inline mat<T, dim> rotate(const mat<T, dim> &m, double radians, const vec<T, dim - 1> &v) {
        rotate(m, radians, vec<T, dim>(v));
    }

    inline double radians(double degree) {
        return pi * (degree / 180.0);
    }

    using mat2 = mat<double, 2>;
    using mat3 = mat<double, 3>;
    using mat4 = mat<double, 4>;

    template<typename T, size_t dim>
    inline std::ostream &operator<<(std::ostream &os, const glm42::mat<T, dim> &obj) {
        for (int col = 0; col < dim; col++) {
            for (int row = 0; row < dim; row++) {
                os << obj.data[col][row] << "\t\t";
            }
            os << std::endl;
        }
        return os;
    }

    template<typename T, size_t dim>
    inline std::ostream &operator<<(std::ostream &os, const glm42::vec<T, dim> &obj) {
        os << "[ ";

        for (int i = 0; i < dim; i++) {
            os << obj.data[i] << ",";
        }

        os << " ]" << std::endl;
        return os;
    }

    template <class T, size_t dim>
    inline vec<T, dim> normalize(const vec<T, dim> &v) {
      T len = std::sqrt(dot(v, v));

      if (len == 0)
        return v;

      vec<T, dim> result;
      for (size_t i = 0; i < dim; ++i)
        result[i] = v[i] / len;
      return result;
    }

    inline vec3 cross(const vec3 &a, const vec3 &b) {
      return vec3{a[1] * b[2] - a[2] * b[1],
                  a[2] * b[0] - a[0] * b[2],
                  a[0] * b[1] - a[1] * b[0]};
    }

    template <class T, size_t dim>
    inline T dot(const vec<T, dim> &a, const vec<T, dim> &b) {
      T result = T(0);
      for (size_t i = 0; i < dim; ++i)
        result += a[i] * b[i];
      return result;
    }

    inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
      vec3 f = normalize(center - eye);
      vec3 s = normalize(cross(f, up));
      vec3 u = cross(s, f);

      mat4 result = mat4::id();

      result.at(0, 0) = s[0];
      result.at(1,0) = s[1];
      result.at(2,0) = s[2];

      result.at(0,1) = u[0];
      result.at(1,1) = u[1];
      result.at(2,1) = u[2];

      result.at(0, 2) = -f[0];
      result.at(1, 2) = -f[1];
      result.at(2, 2) = -f[2];

      result.at(3, 0) = -dot(s, eye);
      result.at(3, 1) = -dot(u, eye);
      result.at(3, 2) = dot(f, eye);

      return result;
    }

    inline mat4 perspective(double near, double far, double fovy, double aspect_ratio) {
      fovy = fovy / 2.0;
      double tg = std::tan(fovy);

      double top_bottom = (tg * near);
      double right_left = (tg * near * aspect_ratio);

      mat4 result = mat4(0.0);

      result.at(0, 0) = (double)(near / right_left);
      result.at(1, 1) = (double)(near / top_bottom);

      result.at(2, 2) = (double)(-1.0 * ((far + near) / (double)(far - near)));
      result.at(3, 2) = (double)(-1.0 * near * 2.0 * far) / (double)(far - near);

      result.at(2, 3) = -1;

      return result;
    }
  }

namespace geom {
    struct Vertex {
        glm42::vec3 pos;
        glm42::vec2 uv;
        glm42::vec3 normal;
        glm42::vec3 color = {0, 0, 0};

        bool operator==(const Vertex& other) {
          return pos == other.pos && uv == other.uv && normal == other.normal;
        }
    };

    inline std::ostream & operator<<(std::ostream &os, const Vertex& vtx) {
      os << "P: ( " << vtx.pos[0] << ", " << vtx.pos[1] << ", " << vtx.pos[2] << " ); ";
      os << "T: ( " << vtx.uv[0] << ", " << vtx.uv[1] << " ); ";
      os << "N: ( " << vtx.normal[0] << ", " << vtx.normal[1] << ", " << vtx.normal[2] << " );";
      return os;
    }

    struct Material {
    };


    struct Mesh {
        std::vector<geom::Vertex> vertexes;
        std::vector<size_t> indexes;

        bool smooth;
    };

}




/*
inline void generateTBN(Geometry & g) {
    for (int i = 0; i < g.indexes_count; i+=3) {
        // Shortcuts for vertices
        unsigned int idx[3] = {
            g.index_data[i],
            g.index_data[i + 1],
            g.index_data[i + 2]
        };

        glm42::vec3 v_pos[3];
        for (int i = 0; i < 3; i++)
            v_pos[i] = {g.vertex_data[idx[i]].Position.X, g.vertex_data[idx[i]].Position.Y, g.vertex_data[idx[i]].Position.Z};


        glm42::vec2 uv[3];
        for (int i = 0; i < 3; i++)
            uv[i] = {g.vertex_data[idx[i]].TextureCoordinate.X, g.vertex_data[idx[i]].TextureCoordinate.Y};

        // Delta pos
        glm42::vec3 d_pos[2] = {
            v_pos[1] - v_pos[0], v_pos[2] - v_pos[0]
        };

        // Delta UV
        glm42::vec2 d_uv[2] = {
            uv[1] - uv[0], uv[2] - uv[0]
        };

        float r = 1.0f / (d_uv[0].x * d_uv[1].y - d_uv[0].y * d_uv[1].x);

		glm42::vec3 tangent = (d_pos[0] * d_uv[1].y - d_pos[1] * d_uv[0].y)*r;
		glm42::vec3 bitangent = (d_pos[1] * d_uv[0].x - d_pos[0] * d_uv[1].x)*r;

        g.vertex_data[idx[0]].tangent = tangent;
        g.vertex_data[idx[1]].tangent = tangent;
        g.vertex_data[idx[2]].tangent = tangent;

        g.vertex_data[idx[0]].bitangent = bitangent;
        g.vertex_data[idx[1]].bitangent = bitangent;
        g.vertex_data[idx[2]].bitangent = bitangent;
    }
}
 */

#endif
