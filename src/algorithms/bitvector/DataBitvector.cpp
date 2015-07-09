#include <algorithms/bitvector/DataBitvector.hpp>

using namespace DataBitvector;

MemStorageT& Bitvector::_getStorageReference(unsigned int position, unsigned int& bitpos) const {
  // calculate positions to fetch bit from
  unsigned int storagepos = position / _getSizeStorageT();
  bitpos = position % _getSizeStorageT();

  if (storagepos >= _storageSize ||  bitpos >= _size) throw "Bitvector: Invalid position for getting storage value was given!";

  return _storage[storagepos];
}

Bitvector::Bitvector(unsigned int size) : _size(size), _storageSize(_calcStorageSize(size)), _storage(new MemStorageT[_storageSize]) {
  // initialize all array elements with zeros
  for (unsigned int i = 0; i < _storageSize; _storage[i++] = 0);
}

Bitvector::Bitvector(const Bitvector& other) : _size(other.getSize()), _storageSize(other.getStorageSize()), _storage(new MemStorageT[_storageSize]) {
  // copy all Storage elements
  for (unsigned int i = 0; i < _storageSize; ++i) {
    _storage[i] = other.getStorage(i);
  }
}

Bitvector& Bitvector::operator=(const Bitvector& other) {
  if (&other == this) return (*this);

  if (_storageSize != other.getStorageSize()) {
    // create new storage array for this instance
    _storageSize = other.getStorageSize();
    _storage.reset(new MemStorageT[_storageSize]);
  }
  _size = other.getSize();

  // copy all Storage elements
  for (unsigned int i = 0; i < _storageSize; ++i) {
    _storage[i] = other.getStorage(i);
  }

  return (*this);
}

Bitvector& Bitvector::operator&=(const Bitvector& rhs) {
  if (&rhs == this) return (*this);

  for (unsigned int i = 0; i < _storageSize && i < rhs.getStorageSize(); ++i) {
    _storage[i] &= rhs.getStorage(i);
  }
  
  // special case: this vector is longer that other
  if (_storageSize > rhs.getStorageSize()) {
    // set all remaining storages to zero
    for (unsigned int i = rhs.getStorageSize(); i < _storageSize; ++i)
      _storage[i] = 0;
  }

  return (*this);
}

Bitvector& Bitvector::operator|=(const Bitvector& rhs) {
  if (&rhs == this) return (*this);

  for (unsigned int i = 0; i < _storageSize && i < rhs.getStorageSize(); ++i) {
    _storage[i] |= rhs.getStorage(i);
  }
  // special case: this vector is longer that other (but keep remaining bits)
  return (*this);
}

StorageT Bitvector::getStorage(unsigned int position) const {
  if (position >= _storageSize) throw "Bitvector: Invalid position in storage array given!";
  return _storage[position];
}

void Bitvector::setBit(unsigned int position) {
  unsigned int bitpos;
  MemStorageT& value = _getStorageReference(position, bitpos);
  value |= (StorageT)(1 << bitpos);
}

bool Bitvector::getBit(unsigned int position) {
  unsigned int bitpos;
  MemStorageT& value = _getStorageReference(position, bitpos);
  return (value & (StorageT)(1 << bitpos)) > 0;
}

unsigned long int Bitvector::getFirstSetBit() const {
  for (unsigned int i = 0; i < _storageSize; ++i) {
    StorageT value = _storage[i];

    if (value > 0) { // if one of the bits set
      
      // check each bit
      for (unsigned int bit = 0; bit < _getSizeStorageT(); ++bit) {
        if ( (value & (StorageT)(1 << bit)) > 0) { // first set bit
          return (bit + i*_getSizeStorageT());
        }
      }
    }
    // continue with next set of bits
  }
  // no bits set
  return noBitSetInBitvector();
}

namespace DataBitvector {
Bitvector operator&(const Bitvector& lhs, const Bitvector& rhs) {
  // create as copy of the longer bitvector
  Bitvector result( (lhs.getSize() >= rhs.getSize() ? lhs : rhs) );
  result &= (lhs.getSize() >= rhs.getSize() ? rhs : lhs);
  return result;
}

Bitvector operator|(const Bitvector& lhs, const Bitvector& rhs) {
  // create as copy of the longer bitvector
  Bitvector result( (lhs.getSize() >= rhs.getSize() ? lhs : rhs) );
  result |= (lhs.getSize() >= rhs.getSize() ? rhs : lhs);
  return result;
}
} // namespace DataBitvector


