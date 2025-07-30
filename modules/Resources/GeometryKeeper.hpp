#ifndef GEOMETRY_KEEPER_HPP
#define GEOMETRY_KEEPER_HPP

#include <memory>

#include <GL/glew.h>

#ifdef TARGET_OS_OSX
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "BuffersCollection.hpp"
#include "Graphic/Object3D.hpp"
#include "Vertex.hpp"
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class GeometryKeeper {
public:
using Ptr = std::shared_ptr<GeometryKeeper>;
	std::unordered_map<std::string, Geometry> geometry;

	void loadGeometryFromFile(const std::string & name, const std::string & objFilename);

	template<typename T>
	T instance(const std::string & name) {
		if (geometry.find(name) != geometry.end()) {
			auto result = T(geometry[name]);
			return result;
		}
		throw std::runtime_error("No available geometry: " + name);
	}

private:
  std::unordered_map<std::string, std::vector<geom::Vertex> > vertexData;
  std::unordered_map<std::string, std::vector<unsigned int> > indexesData;
};


#endif
