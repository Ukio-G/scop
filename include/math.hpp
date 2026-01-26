#ifndef MATH_42_HPP
#define MATH_42_HPP

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>
#include <optional>
#include <ostream>
#include <vector>

namespace glm42 {
    static constexpr float pi = 3.14159265f;

    template<typename T, size_t dim>
    struct vec {
    public:
      template<typename... Args>
        requires (sizeof...(Args) == dim && (std::convertible_to<Args, T> && ...))
      constexpr vec(Args&&... args) : data{static_cast<T>(args)...} {}

        vec(const vec &other) {
            for (unsigned int i = 0; i < dim; i++) {
                data[i] = other.data[i];
            }
        }

        vec &operator=(const vec &other) {
          if (this != &other) {
            for (unsigned int i = 0; i < dim; i++) {
              data[i] = other.data[i];
            }
          }
          return *this;
        }

        glm42::vec<T, dim - 1> reduce()
        {
          glm42::vec<T, dim - 1> result;
          for( unsigned int i = 0; i < dim - 1; i++ )
          {
            result.data[ i ] = data[ i ];
          }
          return result;
        }

        explicit vec(const vec<T, dim - 1> & other) {
          for (unsigned int i = 0; i < dim - 1; i++) {
            data[i] = other.data[i];
          }
          data[dim - 1] = 0;
        }

        explicit vec(T item) {
            for (unsigned int i = 0; i < dim; i++) {
                data[i] = item;
            }
        }

        vec(const vec<T, dim - 1> &other, T last_item) {
            for (unsigned int i = 0; i < dim - 1; i++) {
                data[i] = other.data[i];
            }
            data[dim] = last_item;
        }


        vec() {
            for (unsigned int i = 0; i < dim; i++) {
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
          for (unsigned int i = 0; i < dim; ++i) {
            if (data[i] != other.data[i])
              return false;
          }
          return true;
        }

        T data[dim];
    };

    using vec3 = vec<float, 3>;

    inline vec3 operator*(vec3 lhs, float rhs) {
      lhs[0] *= rhs;
      lhs[1] *= rhs;
      lhs[2] *= rhs;
      return lhs;
    }

    class vec4 : public vec<float, 4> {
    public:
        vec4() : vec(0) {}
        explicit vec4(float i) : vec(i) {
        }

        vec4(const vec4 &other) = default;

        explicit vec4(const vec3 &other) : vec(other) {
            data[3] = 0;
        }

        vec4(float x, float y, float z, float w) : vec(0.0) {
            data[0] = x;
            data[1] = y;
            data[2] = z;
            data[3] = w;
        }
    };

    inline vec4 operator*(const vec4 &lhs, float rhs) {
      vec4 result;

      result[0] = lhs[0] * rhs;
      result[1] = lhs[1] * rhs;
      result[2] = lhs[2] * rhs;
      result[3] = lhs[3] * rhs;

      return result;
    }

    class vec2 : public vec<float, 2> {
    public:
        vec2() : vec(0) {}

        explicit vec2(float i) : vec(i) {
        }

        vec2(const vec2 &other) = default;

        vec2(float x, float y) : vec(0.0) {
            data[0] = x;
            data[1] = y;
        }
    };

    template<class T, size_t dim>
    vec<T, dim> operator+(vec<T, dim> lhs, vec<T, dim> rhs) {
      for (unsigned int i = 0; i < dim; i++)
        lhs[i] += rhs[i];
      return lhs;
    }

    template<class T, size_t dim>
    vec<T, dim> operator-(vec<T, dim> lhs, vec<T, dim> rhs) {
      for (unsigned int i = 0; i < dim; i++)
        lhs[i] -= rhs[i];
      return lhs;
    }

    template<typename T, size_t dim>
    struct mat {
        using vec_type = vec<T, dim>;

        mat(const mat &other) {
            for (unsigned int col = 0; col < dim; col++) {
                for (unsigned int row = 0; row < dim; row++) {
                    data[col][row] = other.data[col][row];
                }
            }
        }

        explicit mat(T item) {
            for (unsigned int col = 0; col < dim; col++) {
                for (unsigned int row = 0; row < dim; row++) {
                    data[col][row] = item;
                }
            }
        }

        mat<T, dim> &operator=(const mat &other) {
            if (this == &other)
                return *this;

            for (unsigned int col = 0; col < dim; col++) {
                for (unsigned int row = 0; row < dim; row++) {
                    data[col][row] = other.data[col][row];
                }
            }

            return *this;
        }

        T det() {
          if constexpr( dim == 2 ) {
            return data[ 0 ][ 0 ] * data[ 1 ][ 1 ] - data[ 0 ][ 1 ] * data[ 1 ][ 0 ];
          }

          if constexpr ( dim > 2 ) {
            auto sign = []( size_t col, size_t row )
            {
              if( ( col + row ) % 2 == 0 )
                return 1;
              return -1;
            };

            T      result = static_cast< T >( 0 );
            size_t row    = 0;

            for( int col = 0; col < dim; col++ )
            {
              glm42::mat< T, dim - 1 > m = minor_matrix( col, row );
              T d = m.det();
              result += d * sign( col, row ) * data[ col ][ row ];
            }

            return result;
          }
        }

        mat< T, dim - 1 > minor_matrix( size_t col, size_t row ) requires( dim > 2 ) {
          mat< T, dim - 1 > result;

          for( size_t i = 0; i < dim; i++ ) {
            for( size_t j = 0; j < dim; j++ ) {
              if( col == i || row == j )
                continue;
              result.data[ i - ( col < i ) ][ j - ( row < j ) ] = data[ i ][ j ];
            }
          }

          return result;
        }

        T minor( size_t col, size_t row )
        {
          if constexpr( dim == 2 )
          {

          }
          else
          {

          }
        }

        mat<T, dim> reverse() {
          mat<T, dim> result;
          auto reverse_det = 1.f / det();
          for( size_t col = 0; col < dim; col++)
          {
            for( size_t row = 0; row < dim; row++)
            {
              auto cofactor = std::pow(-1, minor_matrix( col, row ).det());
              result.data[col][row] = cofactor * reverse_det;
            }
          }
          return result;
        }

        mat() {
            *this = mat(static_cast<T>(0));
        }


        T& at(int col, int row) {
            return data[col][row];
        }

        mat<T, dim> operator*(const mat<T, dim> &other) const {
            mat<T, dim> result;
            for (unsigned int col = 0; col < dim; ++col) {
              for (unsigned int row = 0; row < dim; ++row) {
                for (unsigned int k = 0; k < dim; ++k) {
                  result.data[col][row] += data[k][row] * other.data[col][k];
                }
              }
            }

            return result;
        }

        vec<T, dim> operator*(const vec<T, dim> &other) const {
            vec<T, dim> result(0);
            for (unsigned int row = 0; row < dim; row++) {
                for (unsigned int k = 0; k < dim; k++) {
                    result.data[row] += data[k][row] * other.data[k];
                }
            }

            return result;
        }

        mat<T, dim> operator*(const T number) const {
            mat<T, dim> result;
            for (unsigned int col = 0; col < dim; col++) {
                for (unsigned int row = 0; row < dim; row++) {
                    result.data[col][row] = data[col][row] * number;
                }
            }

            return result;
        }

        mat<T, dim> operator+(const mat<T, dim> &other) const {
            mat<T, dim> result;
            for (unsigned int col = 0; col < dim; col++) {
                for (unsigned int row = 0; row < dim; row++) {
                    result.data[col][row] = data[col][row] + other.data[col][row];
                }
            }

            return result;
        }


        static mat<T, dim> diag(T item) {
            mat<T, dim> result;
            for (unsigned int i = 0; i < dim; ++i) {
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

    // Builds a translation 4 * 4 matrix created from a vector of 3 components
    template<typename T, size_t dim>
    inline mat<T, dim> translate(const mat<T, dim> &m, const vec<T, dim - 1> &v) {
        return translate(m, vec<T, dim>(v));
    }

    template<typename T, size_t dim>
    inline mat<T, dim> translate(const mat<T, dim>& m, vec<T, dim> v) {
      static_assert(dim == 4, "translate is defined for 4x4 matrices only");

      mat<T, dim> result = m;
      for (size_t i = 0; i < 3; ++i) {
        result.data[3][i] += v.data[0] * m.data[i][0] +
                             v.data[1] * m.data[i][1] +
                             v.data[2] * m.data[i][2];
      }
      return result;
    }

    // Builds a scale 4 * 4 matrix created from 3 scalars
    template<typename T, size_t dim>
    inline mat<T, dim> scale(const mat<T, dim>& m, vec<T, dim> v) {
      static_assert(dim == 4, "scale is defined for 4x4 matrices only");

      mat<T, dim> result;
      for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 4; ++j)
          result.data[i][j] = m.data[i][j] * ((j < 3) ? v.data[j] : 1);
      return result;
    }

    // Builds a rotation 4 * 4 matrix created from an axis vector and an angle
    template<typename T, size_t dim>
    inline mat<T, dim> rotate(const mat<T, dim>& m, float radians, const vec<T, dim>& u) {
      static_assert(dim == 4, "rotate is defined for 4x4 matrices only");

      T const a = radians;
      T const c = cos(a);
      T const c_ = 1 - cos(a);
      T const s = sin(a);

      vec3 uc(u[0] * c_, u[1] * c_, u[2] * c_);

      mat<T, dim> result(m);

      result.data[0][0] = u[0] * uc[0] + c;
      result.data[0][1] = u[0] * uc[1] + u[2] * s;
      result.data[0][2] = u[0] * uc[2] - u[1] * s;

      result.data[1][0] = u[1] * uc[0] - u[2] * s;
      result.data[1][1] = u[1] * uc[1] + c;
      result.data[1][2] = u[1] * uc[2] + u[0] * s;

      result.data[2][0] = u[2] * uc[0] + u[1] * s;
      result.data[2][1] = u[2] * uc[1] - u[0] * s;
      result.data[2][2] = u[2] * uc[2] + c;

      return result;
    }

    template<typename T, size_t dim>
    inline mat<T, dim> rotate(const mat<T, dim> &m, float radians, const vec<T, dim - 1> &v) {
        return rotate(m, radians, vec<T, dim>(v));
    }

    inline float radians(float degree) {
        return pi * (degree / 180.0);
    }

    using mat2 = mat<float, 2>;
    using mat3 = mat<float, 3>;
    using mat4 = mat<float, 4>;

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

        os << " ]";
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

    inline mat4 perspective(float near, float far, float fovy, float aspect_ratio) {

      float const tanHalfFovy = tan(fovy / static_cast<float>(2));

      mat4 result = mat4(0.0f);

      result.at(0,0) = static_cast<float>(1) / (aspect_ratio * tanHalfFovy);
      result.at(1,1) = static_cast<float>(1) / (tanHalfFovy);
      result.at(2,2) = - (far + near) / (far - near);
      result.at(2,3) = - static_cast<float>(1);
      result.at(3,2) = - (static_cast<float>(2) * far * near) / (far - near);

      return result;
    }
  }

  namespace geom {
  struct Vertex  {
    glm42::vec3 pos;
    glm42::vec2 uv;
    glm42::vec3 normal;
    glm42::vec3 color = {0.f, 0.f, 0.f};

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

  struct BoundBox {
    struct Dim {
      float min = 0.f;
      float max = 0.f;
    };

    BoundBox& operator=(const BoundBox& other) = default;

    Dim x{};
    Dim y{};
    Dim z{};

    glm42::vec3 center() {
      return {
        0.5f * (x.min + x.max),
        0.5f * (y.min + y.max),
        0.5f * (z.min + z.max)
      };
    }
  };

  inline BoundBox& operator<<(BoundBox& bbox, const geom::Vertex &vtx) {
    auto & [x, y, z] = vtx.pos.data;

    bbox.x.min = std::min(x, bbox.x.min);
    bbox.x.max = std::max(x, bbox.x.max);

    bbox.y.min = std::min(y, bbox.y.min);
    bbox.y.max = std::max(y, bbox.y.max);

    bbox.z.min = std::min(z, bbox.z.min);
    bbox.z.max = std::max(z, bbox.z.max);

    return bbox;
  }

  inline std::ostream & operator<<(std::ostream &os, const BoundBox& bbox) {
    os << "X: (" << bbox.x.min << ", " << bbox.x.max << ")\n";
    os << "Y: (" << bbox.y.min << ", " << bbox.y.max << ")\n";
    os << "Z: (" << bbox.z.min << ", " << bbox.z.max << ")\n";

    return os;
  }


  struct Mesh {
    BoundBox bbox {};
    std::vector<geom::Vertex> vertexes;
    std::vector<unsigned int> indexes;
  };

  }

inline glm42::mat4 to_my_mat(const glm::mat4& gmat) {
  glm42::mat4 result;
  for (int col = 0; col < 4; ++col)
    for (int row = 0; row < 4; ++row)
      result.data[col][row] = gmat[col][row];
  return result;
}


inline glm::mat4 to_glm_mat(const glm42::mat4& gmat) {
  glm::mat4 result;
  for (int col = 0; col < 4; ++col)
    for (int row = 0; row < 4; ++row)
      result[col][row] = gmat.data[col][row];
  return result;
}

inline glm42::mat4 reverse(const glm42::mat4 mat)
{
  auto gmat = glm::inverse( to_glm_mat( mat ) );

  return to_my_mat( gmat );
}


#endif
