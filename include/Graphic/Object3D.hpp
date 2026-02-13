#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <GL/glew.h>
#include <vector>
#include "math.hpp"
#include "BuffersCollection.hpp"
#include "Graphic/TexturesPack.hpp"
#include "Graphic/Shader.hpp"

class Object3D {
public:
	Object3D() = default;
	Object3D( const Object3D &other );
  Object3D(const std::string& name, Geometry geometry, TexturesPack* textures );
  Object3D &operator=( const Object3D &other );
  virtual ~Object3D() = default;

  // Draw API
  void draw();
  void bindToDraw() const;

  // Movement API
  void        setTranslate( const glm42::vec4 &translate );
  glm42::vec4 getTranslate() const;

  void        setScale( const glm42::vec4 &scale );
  glm42::vec4 getScale() const;

  void setRotate( float deg, const glm42::vec3 &axis );
  void setRotate( const glm42::vec4 &rotate );

  glm42::vec4 getRotate() const;

  void updateModelMatrix();
  void initUBO(ShaderProgram& program);

  const glm42::mat4 &getRotationMatrix() const;
  const glm42::mat4 &getTranslateMatrix() const;
  const glm42::mat4 &getScaleMatrix() const;
  const glm42::mat4 &getModelMatrix() const;


  geom::BoundBox getBoundBox() const;
private:
  std::string   name;
  Geometry      geometry;
  TexturesPack* textures = nullptr;
	unsigned int UBO = 0;

  glm42::vec4 rotateVector    = glm42::vec4( 0.0 );
  glm42::vec4 translateVector = glm42::vec4( 0.0 );
  glm42::vec4 scaleVector     = glm42::vec4( 1.0 );

  glm42::mat4 rotationMatrix  = glm42::mat4::id();
  glm42::mat4 translateMatrix = glm42::mat4::id();
  glm42::mat4 scaleMatrix     = glm42::mat4::id();
	glm42::mat4 modelMatrix     = glm42::mat4::id();

  bool dirtyTransform = false;
};

#endif
