#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <GL/glew.h>
#include "math.hpp"
#include "BuffersCollection.hpp"
#include "Graphic/TexturesPack.hpp"
#include "Graphic/Shader.hpp"

class Object3D {
public:
	Object3D() = default;
	Object3D(const Object3D& other) {
		*this = other;
	}

	Object3D& operator=(const Object3D& other) {
		if (this == &other)
			return *this;
		modelMatrix = other.modelMatrix;
		name = other.name;
		geometry = other.geometry;
		textures = other.textures;
		return *this;
	}

	virtual ~Object3D() {
		
	}

	glm42::mat4 modelMatrix = glm42::mat4(1);

	std::string name;

	Geometry geometry;

	// No allocation for TexturesPack - just pointing to TextureKeepers (No need no make shared_ptr)
	TexturesPack* textures = nullptr;

	Object3D(Geometry geometry) : geometry(geometry) { }

	void draw(const ShaderProgram & shaderProgram) {
		bindToDraw(shaderProgram);
		updateModelMatrix();
		glDrawElements(GL_TRIANGLES, (GLint)geometry.vertexes_count, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);  

	}

	void bindToDraw(const ShaderProgram & shaderProgram) {
		bindToDraw();
		shaderProgram.use();
		shaderProgram.setMatrix4d("transform", modelMatrix);
		shaderProgram.setBool("normalMapUsage", textures->normal());
		if (textures->normal())
			shaderProgram.setInt("normalMap", ETT_NORMAL);
	}

	void bindToDraw() {
		if (textures) {
			textures->diffuse()->bind();
			if (auto normal = textures->normal())
				normal->bind(ETT_NORMAL);
		}
		glBindVertexArray(geometry.buffers.VAO);
	}

	const glm42::vec4 &getTranslate() const {
		return translateVector;
	}

	void setTranslate(const glm42::vec3 &translate) {
		auto id = glm42::mat4::id();

		translateVector = translate;
		translateMatrix = glm42::translate(id, translateVector);
	}
	const glm42::vec4 &getRotate() const {
		return rotateVector;
	}
	
	void setScale(const glm42::vec3 &scale) {
		scaleVector = scale;
		scaleMatrix = glm42::mat4(1);

		scaleMatrix = glm42::scale(scaleMatrix, scaleVector);
	}
	
	void setRotate(float deg, const glm42::vec3 &axis) {
		auto id = glm42::mat4(1);
		rotationMatrix = glm42::rotate(id, glm42::radians(deg), axis);
	}

	void setRotate(const glm42::vec3 &rotate) {
		rotateVector = rotate;
		auto id = glm42::mat4(1);



		rotationMatrix = glm42::rotate(id, glm42::radians(rotateVector[0]), glm42::vec3(1.0, 0.0, 0.0));
		rotationMatrix = glm42::rotate(rotationMatrix, glm42::radians(rotateVector[1]), glm42::vec3(0.0, 1.0, 0.0));
		rotationMatrix = glm42::rotate(rotationMatrix, glm42::radians(rotateVector[2]), glm42::vec3(0.0, 0.0, 1.0));
	}

	void updateModelMatrix() {
		modelMatrix = rotationMatrix * translateMatrix * scaleMatrix;
	}

	void setRotateMatrix(const glm42::mat4 & mat) {
		rotationMatrix = mat;
	}

	const glm42::mat4 &getRotationMatrix() const {
		return rotationMatrix;
	}

	const glm42::mat4 &getTranslateMatrix() const {
		return translateMatrix;
	}
	
	const glm42::mat4 &getScaleMatrix() const {
		return scaleMatrix;
	}
private:
	glm42::vec4 rotateVector = glm42::vec3(0.0);
	glm42::vec4 translateVector = glm42::vec3(0.0);
	glm42::vec4 scaleVector = glm42::vec3(1.0);

	glm42::mat4 rotationMatrix = glm42::mat4(1.0);
	glm42::mat4 translateMatrix = glm42::mat4(1.0);
	glm42::mat4 scaleMatrix = glm42::mat4(1.0);
};

#endif
