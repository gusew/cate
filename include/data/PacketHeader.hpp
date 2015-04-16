#ifndef PACKET_HEADER_INCLUDED
#define PACKET_HEADER_INCLUDED

#include <memory>
#include <vector>
#include <data/PacketHeaderAtom.hpp>

namespace Generic {

typedef std::vector<std::unique_ptr<PacketHeaderAtom>> PacketHeaderLine;

typedef std::vector<std::unique_ptr<PacketHeaderLine>> PacketHeaderSet;

}
#endif
