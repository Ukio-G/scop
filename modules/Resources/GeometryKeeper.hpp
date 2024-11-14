#ifndef GEOMETRY_KEEPER_HPP
#define GEOMETRY_KEEPER_HPP

#include "Graphic/Line.hpp"
#include "Graphic/LineStrip.hpp"
#include <memory>

#include <GL/glew.h>
#include <GL/gl.h>


#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "Vertex.hpp"
#include "Graphic/Object3D.hpp"
#include "BuffersCollection.hpp"

class GeometryKeeper {
public:
using Ptr = std::shared_ptr<GeometryKeeper>;
	std::unordered_map<std::string, Geometry> geometry;

	void loadGeometryFromFile(const std::string & name, const std::string & objFilename);
	void newGeometry(const std::string &name, const Vertex *vertexData, const int *indexes, size_t vertexes_count,
					size_t indexes_count);
	void newGeometry(const std::string &name, const std::vector<Vertex> &data, const std::vector<int> &indexes);

	void newLineGeometry(const std::string & name, const glm42::vec3 & start ,const glm42::vec3 & end);
	void newLineStripGeometry(const std::string & name, const std::vector<glm42::vec3> & points);

	template<typename T>
	T instance(const std::string & name) {
		if (geometry.find(name) != geometry.end()) {
			auto result = T(geometry[name]);
			return result;
		}
		throw std::runtime_error("No available geometry: " + name);
	}

private:
	std::unordered_map<std::string, std::vector<Vertex> > vertexData;
	std::unordered_map<std::string, std::vector<unsigned int> > indexesData;
};


#endif
