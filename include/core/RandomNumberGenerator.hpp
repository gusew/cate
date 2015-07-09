#ifndef RANDOMNUMBERGENERATOR_INCLUDED
#define RANDOMNUMBERGENERATOR_INCLUDED

#include <random>
#include <memory>
#include <data/RandomDistribution.hpp>
#include <generics/VarValue.hpp>

/** Choose a generator engine type (for values <= 32 bit): */
//typedef std::default_random_engine GeneratorEngine;

//typedef std::linear_congruential_engine<UIntType, a, c, m> GeneratorEngine;
typedef std::minstd_rand GeneratorEngine32; // a=48271, c=0, m=2147483647
//typedef std::minstd_rand0 GeneratorEngine; // a=16807, c=0, m=2147483647

//typedef std::subtract_with_carry_engine<UIntType, w, s, r> GeneratorEngine;
//typedef std::ranlux24_base GeneratorEngine; // w=24, s=10, r=24
//typedef std::ranlux48_baseGeneratorEngine; // w=48, s=5, r=12

//typedef std::mersenne_twister_engine<UIntType, w, n, m, m, a, u, d, s, b, t, c, l, f> GeneratorEngine;
//typedef std::mt19937 GeneratorEngine; // w=32, n=624, m=397, r=31, a=0x9908b0df, u=11, ...
//typedef std::mt19937_64 GeneratorEngine; // w=64, n=312, m=156, r=31, a=0xb5026f5aa96619e9, u=29, ...

/** Generator type for uniform distribution values with size > 32 bits. */
typedef gmp_randclass GeneratorEngineBig;

/**
 * This class is responsible for the generation of random numbers in a variable
 * data type format. Therefore it is initiated with a seed value and a random
 * distribution to provide the numbers for.
 */
class RandomNumberGenerator {
  /** Amount of relevant bits that output values shall have. */
  unsigned int _bitlength;
  /** Is constructed according to '_bitlength' in order to keep output values in valid range. */
  Generic::VarValue _bitmask;

  /** Generates the pseudo random values that are used to output a random sample number. */
  GeneratorEngine32 _engine32;

  /** Generates the pseudo random values for uniform distribution with more than 32 bits. */
  GeneratorEngineBig _engineBig;
  
  /** All supported types of distributions by the number generator. */
  enum DistributionType { CONSTANT, UNIFORM, NORMAL, LOGNORMAL, EXPONENTIAL, CAUCHY, PARETO };

  /** Distribution to use for number generation. */
  DistributionType _distribType;

  /** Constant number used for constant distribution. */
  Generic::VarValue _distribConstant;
  /** Range values for uniform distribution. */
  Generic::VarValue _uniformMinVal, _uniformMaxVal;
  /** Normal distribution instance. */
  std::normal_distribution<double> _distribNormal;
  /** Lognormal distribution instance. */
  std::lognormal_distribution<double> _distribLognormal;
  /** Exponential distribution instance. */
  std::exponential_distribution<double> _distribExponential;
  /** Cauchy distribution instance. */
  std::cauchy_distribution<double> _distribCauchy;
  /** Pareto distribution settings. */
  double _paretoScale, _paretoShape;
  Generic::VarValue _paretoOffset;

  /** Does the construction of '_bitmask'. */
  void _createBitmask();

public:
  RandomNumberGenerator(unsigned int seed, unsigned int bits, const std::unique_ptr<RandomDistribution>& distrib);
  
  ~RandomNumberGenerator() {}

  /** Get a single random value according to configuration. */
  void getSample(Generic::VarValue& value);
};

#endif

