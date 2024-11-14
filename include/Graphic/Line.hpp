#ifndef __LINE__H__
#define __LINE__H__

#include "Graphic/Object3D.hpp"
#include "Vertex.hpp"

class Line : public Object3D {
    public:
    
    Line() = default;

    Line& operator=(const Line& other) {
        if(this == &other)
            return *this;
        Object3D::operator=(other);
        return *this;
    }

	Line (Geometry geometry) : Object3D(geometry) { }

    Line(const Line &other) : Object3D(other) {}

    ~Line() {    }

	void draw(const ShaderProgram & shaderProgram) {
		bindToDraw(shaderProgram);
		updateModelMatrix();
		glDrawArrays(GL_LINE_LOOP, 0, 2);
		glBindVertexArray(0);
	}

	void bindToDraw(const ShaderProgram & shaderProgram) {
		bindToDraw();
		shaderProgram.use();
		shaderProgram.setMatrix4d("transform", modelMatrix);
	}

	void bindToDraw() {
		glBindVertexArray(geometry.buffers.VAO);
	}
};

#endif  //!__LINE__H__