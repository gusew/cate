#include <core/HeaderGenerator.hpp>
#include <memory>


bool HeaderGenerator::configure(const FieldStructureSet& fields, const RandomHeaderConfiguration& config) {
  _configured = false;
  _generators.clear(); // reset generators

  if (fields.size() != config.seeds.size() || fields.size() != config.distributions.size())
    throw "Field structure does not match amount of random generator seed values or -distributions.";

  RandomGeneratorSeedSet::const_iterator iterSeeds(config.seeds.begin());
  RandomDistributionSet::const_iterator iterDistribs(config.distributions.begin());
  FieldStructureSet::const_iterator iterFields(fields.begin());

  while (iterSeeds != config.seeds.end() && 
    iterDistribs != config.distributions.end() && 
    iterFields != fields.end()) {
   
    if (*iterFields > 32) throw "Currently, values with more than 32 bits are not supported for header generation."; // TODO implement support of larger VarValues
    
    std::unique_ptr<RandomNumberGenerator> generator(new RandomNumberGenerator(*iterSeeds, *iterFields, *iterDistribs));
    _generators.push_back(std::move(generator));

    ++iterSeeds;
    ++iterDistribs;
    ++iterFields;
  }
  _configured = true;

  return _configured;
}

void HeaderGenerator::generateHeaders(unsigned int amount, Generic::PacketHeaderSet& output) {
  using namespace Generic;

  if (!_configured) throw "Please configure the HeaderGenerator before using it.";
  
  output.clear(); // clear before add new headers

  for (unsigned int headerCnt = 0; headerCnt < amount; ++headerCnt) { // outer loop
    std::unique_ptr<PacketHeaderLine> headerLine(new PacketHeaderLine);

    // iterate over each field
    for (GeneratorSet::iterator iter(_generators.begin()); iter != _generators.end(); ++iter) {
      VarValue rndValue;
      (*iter)->getSample(rndValue);
      std::unique_ptr<PacketHeaderAtom> field(new PacketHeaderAtom(rndValue));
      headerLine->push_back(std::move(field));
    }

    output.push_back(std::move(headerLine)); // add all fields as header
  }
}


