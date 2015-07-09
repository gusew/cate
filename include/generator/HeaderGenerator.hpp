#ifndef HEADERGENERATOR_INCLUDED
#define HEADERGENERATOR_INCLUDED

#include <vector>
#include <memory>
#include <configuration/Benchmark.hpp>
#include <generics/PacketHeader.hpp>
#include <generator/RandomNumberGenerator.hpp>

/** Contains all independent number generators each for one header field. */
typedef std::vector<std::unique_ptr<RandomNumberGenerator>> GeneratorSet;

/**
 * Responsible for generation of random values for synthetic header fields.
 * Before using the HeaderGenerator it needs to be configured with the random
 * distributions to use and the seed number for the generators. Independent
 * instances of generators will then be constructed to enable generate headers.
 */
class HeaderGenerator {
  /** is set to true after the generator was configured */
  bool _configured;

  /** holds all generators for header values generation */
  GeneratorSet _generators;

public:
  HeaderGenerator() : _configured(false), _generators() {}
  ~HeaderGenerator() {}
  
  /**
   * Specify the field structure of the header fields to generate and which
   * random distribution should be used per header field. Also, seed values
   * for the random generators are set to allow reproducibility of tests.
   *
   * @param fields header data field structure
   * @param config random generator configuration
   * @return true, if configuration is valid and set successfully
   */
  bool configure(const FieldStructureSet& fields, const RandomHeaderConfiguration& config);

  /**
   * Generates header values according to previously set configuration and
   * returns them in the specified amount in a handed set.
   *
   * @param amount number of headers to generate
   * @param output set for headers to generate (will be emptied before generation)
   */
  void generateHeaders(unsigned int amount, Generic::PacketHeaderSet& output);
};

#endif

