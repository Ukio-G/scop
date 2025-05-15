#ifndef MATH_42_HPP
#define MATH_42_HPP

#include <ostream>
#include <cstddef>

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

        explicit vec(const vec<T, dim - 1> &other) {
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

        T data[dim];
    };

    class vec3 : public vec<double, 3> {
    public:
        explicit vec3(double i) : vec(i) {
        }

        vec3(const vec3 &other) = default;

        vec3(double x, double y, double z) : vec(0.0) {
            data[0] = x;
            data[1] = y;
            data[2] = z;
        }
    };

    class vec4 : public vec<double, 4> {
    public:
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
        explicit vec2(double i) : vec(i) {
        }

        vec2(const vec2 &other) = default;

        vec2(double x, double y) : vec(0.0) {
            data[0] = x;
            data[1] = y;
        }
    };

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
    mat<T, dim> translate(const mat<T, dim> &m, vec<T, dim> v) {
    }

    template<typename T, size_t dim>
    mat<T, dim> scale(const mat<T, dim> &m, vec<T, dim> v) {
    }

    template<typename T, size_t dim>
    mat<T, dim> rotate(const mat<T, dim> &m, double radians, const vec<T, dim> &v) {
    }

    template<typename T, size_t dim>
    mat<T, dim> rotate(const mat<T, dim> &m, double radians, const vec<T, dim - 1> &v) {
        rotate(m, radians, vec<T, dim>(v));
    }

    double radians(double degree) {
        return pi * (degree / 180.0);
    }

    using mat2 = mat<double, 2>;
    using mat3 = mat<double, 3>;
    using mat4 = mat<double, 4>;

    template<typename T, size_t dim>
    std::ostream &operator<<(std::ostream &os, const glm42::mat<T, dim> &obj) {
        for (int col = 0; col < dim; col++) {
            for (int row = 0; row < dim; row++) {
                os << obj.data[col][row] << "\t\t";
            }
            os << std::endl;
        }
        return os;
    }

    template<typename T, size_t dim>
    std::ostream &operator<<(std::ostream &os, const glm42::vec<T, dim> &obj) {
        os << "[ ";

        for (int i = 0; i < dim; i++) {
            os << obj.data[i] << ",";
        }

        os << " ]" << std::endl;
        return os;
    }

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
