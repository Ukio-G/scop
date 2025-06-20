#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

#include "math.hpp"

bool operator==(const glm42::mat4& lhs, const glm::mat4& rhs) {
  constexpr float epsilon = 1e-6f;

  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      if (std::abs(lhs.data[col][row] - rhs[col][row]) > epsilon) {
        return false;
      }
    }
  }
  return true;
}

void print_matrix(const glm::mat4& m) {
  std::cout << "GLM Matrix:\n";
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      std::cout << std::setw(10) << std::setprecision(4) << m[col][row] << ' ';
    }
    std::cout << '\n';
  }
}

void print_matrix(const glm42::mat4& m) {
  std::cout << "My Matrix:\n";
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      std::cout << std::setw(10) << std::setprecision(4) << m.data[col][row] << ' ';
    }
    std::cout << '\n';
  }
}


void fill_random(glm::mat4& mat, std::mt19937& rng, std::uniform_real_distribution<float>& dist) {
  for (int col = 0; col < 4; ++col)
    for (int row = 0; row < 4; ++row)
      mat[col][row] = dist(rng);
}

glm42::mat4 to_my_mat(const glm::mat4& gmat) {
  glm42::mat4 result;
  for (int col = 0; col < 4; ++col)
    for (int row = 0; row < 4; ++row)
      result.data[col][row] = gmat[col][row];
  return result;
}


void test_random_matrix_multiplication(int seed = 42) {
  std::mt19937 rng(seed);
  std::uniform_real_distribution<float> dist(-10.0f, 10.0f);

  glm::mat4 a, b;
  fill_random(a, rng, dist);
  fill_random(b, rng, dist);

  glm::mat4 glm_result = a * b;

  glm42::mat4 my_a = to_my_mat(a);
  glm42::mat4 my_b = to_my_mat(b);

  if (my_a != a || my_b != b)
  {
    throw std::runtime_error("Matrix conversion failed");
  }

  glm42::mat4 my_result = my_a * my_b;

  if (glm_result != my_result) {
    print_matrix(glm_result);
    std::cout << "-------\n";
    print_matrix(my_result);

    std::cout << "My A:\n";
    print_matrix(a);

    std::cout << "My B:\n";
    print_matrix(b);
    throw std::runtime_error("Random matrix multiplication test failed");
  }
}

void rotate_test(float angle, float x, float y, float z)
{
  glm::mat4 glm_mat(1);
  glm42::mat4 my_mat = glm42::mat4::id();

  if (glm_mat != my_mat)
  {
    throw std::runtime_error("Matrix compare before rotation failed");
  }

  glm_mat = glm::rotate(glm_mat, angle, glm::vec3(x, y, z));
  my_mat = glm42::rotate(my_mat, angle, glm42::vec3(x, y, z));

  print_matrix(glm_mat);
  std:: cout << "-------" << std::endl;
  print_matrix(my_mat);

  if (glm_mat != my_mat)
  {
    std::stringstream ss;
    ss << "angle: " << angle << ", axis(" << x << ", " << y << ", " << ", " << z << ")";
    throw std::runtime_error("Incorrect rotation: " + ss.str());
  }
}


inline float to_radians(float angle) {
  return M_PI * angle / 180.f;
}

int main()
{

  glm::mat4 id(1);
  glm42::mat4 id42 = glm42::mat4::id();

  if (id != id42)
  {
    throw std::runtime_error("Incorrect == oerator");
  }

  test_random_matrix_multiplication(10);
  test_random_matrix_multiplication(20);

  rotate_test(to_radians(30.f), 0, 0, 1);
  rotate_test(to_radians(30.f), 0, 1, 0);
  rotate_test(to_radians(30.f), 1, 0, 0);

  return 0;
}