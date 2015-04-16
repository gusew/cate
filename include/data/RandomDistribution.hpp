#ifndef RANDOMDISTRIBUTION_INCLUDED
#define RANDOMDISTRIBUTION_INCLUDED

#include <limits>
#include <data/VarValue.hpp>

/** Abstract base type for all different random distributions. */
struct RandomDistribution {
  enum Type { CONSTANT, UNIFORM, NORMAL, LOGNORMAL, EXPONENTIAL, CAUCHY, PARETO };

  virtual ~RandomDistribution() {}
  virtual Type getType() const = 0;
};

/** Represents the configuration of a constant distribution (ignores random generator). */
struct RandomDistConstant : RandomDistribution {
  Generic::VarValue value;

  RandomDistConstant(Generic::VarValue& val) : value(val) {}

  inline ~RandomDistConstant() override {}
  inline Type getType() const override { return RandomDistribution::CONSTANT; }
};

/** Represents the configuration of a uniform distribution. */
struct RandomDistUniform : RandomDistribution {
  Generic::VarValue minValue;
  Generic::VarValue maxValue;

  RandomDistUniform(Generic::VarValue& max) : minValue(0), maxValue(max) {}
  RandomDistUniform(Generic::VarValue& min, Generic::VarValue& max) : minValue(min), maxValue(max) {}

  inline ~RandomDistUniform() override {}
  inline Type getType() const override { return RandomDistribution::UNIFORM; }
};

/** Represents the configuration of a normal distribution. */
struct RandomDistNormal : RandomDistribution {
  Generic::VarValue mean;
  Generic::VarValue stddev;

  RandomDistNormal(Generic::VarValue& meanval, Generic::VarValue dev) : mean(meanval), stddev(dev) {}

  inline ~RandomDistNormal() override {}
  inline Type getType() const override { return RandomDistribution::NORMAL; }
};

/** Represents the configuration of a lognormal distribution. */
struct RandomDistLognormal : RandomDistribution {
  Generic::VarValue m;
  Generic::VarValue s;

  RandomDistLognormal(Generic::VarValue& mval, Generic::VarValue& sval) : m(mval), s(sval) {}

  inline ~RandomDistLognormal() override {}
  inline Type getType() const override { return RandomDistribution::LOGNORMAL; }
};

/** Represents the configuration of a exponential distribution (analogous to geometric distrib.). */
struct RandomDistExponential : RandomDistribution {
  double lambda;

  RandomDistExponential(double lval) : lambda(lval) {}

  inline ~RandomDistExponential() override {}
  inline Type getType() const override { return RandomDistribution::EXPONENTIAL; }
};

/** Represents the configuration of a cachy distribution. */
struct RandomDistCauchy : RandomDistribution {
  Generic::VarValue a;
  double b;

  RandomDistCauchy(Generic::VarValue& aval, double bval) : a(aval), b(bval) {}

  inline ~RandomDistCauchy() override {}
  inline Type getType() const override { return RandomDistribution::CAUCHY; }
};

/** Represents the configuration of a pareto distribution. */
struct RandomDistPareto : RandomDistribution {
  double scale;
  double shape;
  Generic::VarValue offset;

  RandomDistPareto(double scaleval, double shapeval, Generic::VarValue& offval) : scale(scaleval), shape(shapeval), offset(offval) {}

  inline ~RandomDistPareto() override {}
  inline Type getType() const override { return RandomDistribution::PARETO; }
};


#endif

