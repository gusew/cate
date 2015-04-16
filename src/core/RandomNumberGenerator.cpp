#include <core/RandomNumberGenerator.hpp>
#include <cmath>

void RandomNumberGenerator::_createBitmask() {
  Generic::VarValue onemore(1);
  onemore = onemore << _bitlength;
  _bitmask = onemore - 1;
}

RandomNumberGenerator::RandomNumberGenerator(unsigned int seed, unsigned int bits, const std::unique_ptr<RandomDistribution>& distrib) : 
  _bitlength(bits), _bitmask(0), _engine32(seed), _engineBig(gmp_randinit_default) {

  _createBitmask();

  if (distrib->getType() == RandomDistribution::UNIFORM) {
    RandomDistUniform* uniDist = static_cast<RandomDistUniform*>(distrib.get());
    
    _distribType = DistributionType::UNIFORM;
    _engineBig.seed(seed);
    _uniformMinVal = uniDist->minValue;
    _uniformMaxVal = uniDist->maxValue;
    
    // sanity check
    if (_uniformMinVal > _uniformMaxVal)
      throw "Range for uniform random distribution is invalid (min greater than max)!";
  } 
  else if (distrib->getType() == RandomDistribution::CONSTANT) {
    RandomDistConstant* constDist = static_cast<RandomDistConstant*>(distrib.get());
    
    _distribType = DistributionType::CONSTANT;
    _distribConstant = constDist->value;
  } 
  else if (distrib->getType() == RandomDistribution::NORMAL) {
    RandomDistNormal* dist = static_cast<RandomDistNormal*>(distrib.get());
    
    _distribType = DistributionType::NORMAL;
    _distribNormal = std::normal_distribution<double>(dist->mean.get_d(), dist->stddev.get_d());
  }
  else if (distrib->getType() == RandomDistribution::LOGNORMAL) {
    RandomDistLognormal* dist = static_cast<RandomDistLognormal*>(distrib.get());
    
    _distribType = DistributionType::LOGNORMAL;
    _distribLognormal = std::lognormal_distribution<double>(dist->m.get_d(), dist->s.get_d());
  }
  else if (distrib->getType() == RandomDistribution::EXPONENTIAL) {
    RandomDistExponential* dist = static_cast<RandomDistExponential*>(distrib.get());
    
    _distribType = DistributionType::EXPONENTIAL;
    _distribExponential = std::exponential_distribution<double>(dist->lambda);
  }
  else if (distrib->getType() == RandomDistribution::CAUCHY) {
    RandomDistCauchy* dist = static_cast<RandomDistCauchy*>(distrib.get());
    
    _distribType = DistributionType::CAUCHY;
    _distribCauchy = std::cauchy_distribution<double>(dist->a.get_d(), dist->b);
  }
  else if (distrib->getType() == RandomDistribution::PARETO) {
    RandomDistPareto* dist = static_cast<RandomDistPareto*>(distrib.get());
    
    _distribType = DistributionType::PARETO;
    _paretoScale = dist->scale;
    _paretoShape = dist->shape;
    _paretoOffset = dist->offset;
  }
 
  else 
    throw "RandomNumberGenerator: Given random distribution is currently not supported.";
}

void RandomNumberGenerator::getSample(Generic::VarValue& value) {
  switch (_distribType) {
    case DistributionType::CONSTANT:
      value = _distribConstant;
      break;
    case DistributionType::UNIFORM:
      value = (_engineBig.get_z_range(_uniformMaxVal - _uniformMinVal + 1) + _uniformMinVal) & _bitmask;
      break;
    case DistributionType::NORMAL:
      value = _distribNormal(_engine32) & _bitmask;
      break;
    case DistributionType::LOGNORMAL:
      value = _distribLognormal(_engine32) & _bitmask;
      break;
    case DistributionType::EXPONENTIAL:
      value = _distribExponential(_engine32) & _bitmask;
      break;
    case DistributionType::CAUCHY:
      value = _distribCauchy(_engine32) & _bitmask;
      break;
    case DistributionType::PARETO: {
      double q = 1.0 - (double)_engine32() / 0xFFFFFFFF;
      value = _paretoScale / pow(q, 1.0 / _paretoShape);
      value += _paretoOffset;
      value &= _bitmask;
      break;
    }
    default: 
      throw "Chosen distribution is currently not supported.";
  }
}


