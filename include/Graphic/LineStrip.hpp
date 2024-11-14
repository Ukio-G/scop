#ifndef __LINESTRIP__H__
#define __LINESTRIP__H__


#include "Graphic/Object3D.hpp"
#include "Vertex.hpp"


class LineStrip : public Object3D {
    public:

    LineStrip() = default;

    LineStrip& operator=(const LineStrip& other) {
        if(this == &other)
            return *this;
        Object3D::operator=(other);
        return *this;
    }

	LineStrip (Geometry geometry) : Object3D(geometry) { }

    LineStrip(const LineStrip &other) : Object3D(other) {}

    ~LineStrip() {

    }

	void draw(const ShaderProgram & shaderProgram) {
		bindToDraw(shaderProgram);
		updateModelMatrix();
		glDrawArrays(GL_LINE_LOOP, 0, (GLint)geometry.vertexes_count);
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


#endif  //!__LINESTRIP__H__