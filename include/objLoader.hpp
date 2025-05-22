#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include "math.hpp"

namespace obj {
    enum RecordType : size_t {
        Comment,
        VertexPos,
        VertexUV,
        VertexNormal,
        Face,
        Material,
        Smooth,
        Group,
        Object,
        Undefined
    };

    class Loader {
        bool LoadFile(const std::string & string);


        RecordType getLineType(const std::string & line) {
            // Skip leading spaces
            size_t pos = 0;
            while (line[pos] == ' ')
                pos++;

            if (line[pos] == '#') return RecordType::Comment;
            if (line[pos] == 'v') return RecordType::VertexPos;
            if (line[pos] == 'v' && line[pos + 1] == 't') return RecordType::VertexUV;
            if (line[pos] == 'v' && line[pos + 1] == 'n') return RecordType::VertexNormal;
            if (line[pos] == 'f') return RecordType::Face;
            if (line[pos] == 'm') return RecordType::Material;
            if (line[pos] == 's') return RecordType::Smooth;
            if (line[pos] == 'g') return RecordType::Group;
            if (line[pos] == 'o') return RecordType::Object;

            return Undefined;
        }


        template<size_t T>
        void parseLine(const std::string & line);


        void parseVectorPos(const std::string & line) {

        }

        void parseVectorUV(const std::string & line);

        void parseVectorNormal(const std::string & line);

        void parseFace(const std::string & line);

        void parseMaterial(const std::string & line);

        void parseSmooth(const std::string & line);

        void parseType(const std::string & line);

        void parseObject(const std::string & line);

        void LoadNextMesh(std::ifstream & file) {

            for (std::string line; std::getline(file, line);) {

                RecordType type = getLineType(line);
                if (type == RecordType::Comment)
                    continue;

                if (type == RecordType::Undefined)
                    throw std::runtime_error("Undefined record while parsing OBJ file");

                switch (type) {
                    case RecordType::VertexPos: parseVectorPos(line); break;
                    case RecordType::VertexUV: parseVectorUV(line); break;
                    case RecordType::VertexNormal: parseVectorNormal(line); break;
                    case RecordType::Face: parseFace(line); break;
                    case RecordType::Material: parseMaterial(line); break;
                    case RecordType::Smooth: parseSmooth(line); break;
                    case RecordType::Group: parseType(line); break;
                    case RecordType::Object: parseObject(line); break;
                }

            }
        }



    public:
        std::vector<geom::Mesh> meshes;
        std::vector<geom::Vertex> vertices;

    private:
        std::vector<glm42::vec3> positions;
        std::vector<glm42::vec3> normals;
        std::vector<glm42::vec2> texcoords;


    };

}


#endif //OBJLOADER_HPP
