#ifndef INCLUDED_VEC_H
#define INCLUDED_VEC_H

#include "BitStream.h"
#include <glm/glm.hpp>

inline RakNet::BitStream& operator<<(RakNet::BitStream& out, glm::vec3& in) {
    out.WriteVector(in.x, in.y, in.z);
    return out;
}

inline RakNet::BitStream& operator>>(RakNet::BitStream& in, glm::vec3& out) {
    in.ReadVector(out.x, out.y, out.z);
    return in;
}

inline RakNet::BitStream& operator<<(RakNet::BitStream& out, glm::vec4& in) {
    out.WriteVector(in.x, in.y, in.z);
    out.Write(in.w);
    return out;
}

inline RakNet::BitStream& operator>>(RakNet::BitStream& in, glm::vec4& out) {
    in.ReadVector(out.x, out.y, out.z);
    in.Read(out.w);
    return in;
}
#endif
