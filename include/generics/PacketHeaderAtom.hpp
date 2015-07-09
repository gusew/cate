#ifndef PACKET_HEADER_ATOM_INCLUDED
#define PACKET_HEADER_ATOM_INCLUDED

#include <string>
#include <generics/VarValue.hpp>

namespace Generic {

struct PacketHeaderAtom {
	VarValue value;
	PacketHeaderAtom(const VarValue& val) : value(val) { }
	PacketHeaderAtom(unsigned int val) : value(val) { }
	PacketHeaderAtom(const std::string& val) : value(val) { }
	PacketHeaderAtom(double val) : value(val) { }
};

} // namespace Generic

#endif
