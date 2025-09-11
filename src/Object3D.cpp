// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Graphic/Object3D.hpp"
#include "math.hpp"


Object3D::Object3D( const Object3D& other ) {
  *this = other;
}


Object3D::Object3D( const std::string& name, Geometry geometry, TexturesPack* textures )
    : name( name )
    , geometry( geometry )
    , textures( textures )
{
  useTexture = true;
  textureColorLerpFactor = 0.5f;
}

Object3D& Object3D::operator=( const Object3D& other )
{
  if( this == &other )
    return *this;

  name                   = other.name;
  geometry               = other.geometry;
  textures               = other.textures;
  useTexture             = other.useTexture;
  textureColorLerpFactor = other.textureColorLerpFactor;

  setTranslate( other.translateVector );
  setScale( other.scaleVector );
  setRotate( other.rotateVector );

  updateModelMatrix();
  return *this;
}


void Object3D::draw( const ShaderProgram& shaderProgram ) {
  if (dirtyTransform == true)
    updateModelMatrix();

  bindToDraw( shaderProgram );
  glDrawElements( GL_TRIANGLES, ( GLint )geometry.indexes_count, GL_UNSIGNED_INT, 0 );
  glBindVertexArray( 0 );
}


void Object3D::bindToDraw( const ShaderProgram& shaderProgram ) {
  if( textures ) {
    if (auto diffuse = textures->diffuse()) {
      diffuse->bind();
    }
  }

  glBindVertexArray( geometry.buffers.VAO );

  shaderProgram.setMatrix4d( "transform", modelMatrix );
  shaderProgram.setBool( "u_hasTexture", useTexture );
}


glm42::vec4 Object3D::getTranslate() const {
  return translateVector;
}


void Object3D::setTranslate( const glm42::vec4& translate ) {
  translateVector = translate;
  translateMatrix = glm42::mat4::id();
  translateMatrix = glm42::translate( translateMatrix, glm42::vec4( translateVector ) );

  dirtyTransform = true;
}


glm42::vec4 Object3D::getRotate() const {
  return rotateVector;
}


void        Object3D::setScale( const glm42::vec4& scale ) {
  scaleVector = scale;
  scaleMatrix = glm42::mat4::id();
  scaleMatrix = glm42::scale( scaleMatrix, glm42::vec4( scaleVector ) );

  dirtyTransform = true;
}


glm42::vec4 Object3D::getScale() const {
  return scaleVector;
}

void Object3D::setRotate( float deg, const glm42::vec3& axis ) {
  auto id        = glm42::mat4( 1 );
  rotationMatrix = glm42::rotate( id, glm42::radians( deg ), axis );

  dirtyTransform = true;
}


void Object3D::setRotate( const glm42::vec4& rotate ) {
  rotateVector = rotate;
  auto id      = glm42::mat4::id();

  rotationMatrix = glm42::rotate( id, glm42::radians( rotateVector[ 0 ] ), glm42::vec3( 1.0, 0.0, 0.0 ) );
  rotationMatrix = rotationMatrix * glm42::rotate( id, glm42::radians( rotateVector[ 1 ] ), glm42::vec3( 0.0, 1.0, 0.0 ) );
  rotationMatrix = rotationMatrix * glm42::rotate( id, glm42::radians( rotateVector[ 2 ] ), glm42::vec3( 0.0, 0.0, 1.0 ) );

  dirtyTransform = true;
}


void Object3D::updateModelMatrix() {
  modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;

  dirtyTransform = false;
}


const glm42::mat4& Object3D::getRotationMatrix() const {
  return rotationMatrix;
}


const glm42::mat4& Object3D::getTranslateMatrix() const {
  return translateMatrix;
}


const glm42::mat4& Object3D::getScaleMatrix() const {
  return scaleMatrix;
}

const glm42::mat4& Object3D::getModelMatrix() const {
  return modelMatrix;
}

geom::BoundBox Object3D::getBoundBox() const {
  return geometry.bbox;
}

void Object3D::setUseTexture(bool use_texture)
{
  useTexture = use_texture;
}