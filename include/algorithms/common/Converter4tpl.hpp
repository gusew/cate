#ifndef CONVERTER_4TPL_INCLUDED
#define CONVERTER_4TPL_INCLUDED

#include <generics/RuleSet.hpp>
#include <generics/PacketHeader.hpp>
#include <algorithms/common/Data4tpl.hpp>

/** Handles the conversion of values with variable length to target five-tuple type. */
class Converter4tpl {

public:
  /** Provide just only static functions. */
  Converter4tpl() = delete;

  /**
   * Helper function to convert each rule atom from generic rule structure
   * to the rule implementation used by five-tuple algorithms.
   *
   * @param varAtom generic rule atom structure
   * @param specAtom reference to five-tuple rule atom object
   */
  template <typename T>
  static void convertRuleAtom(Generic::RuleAtom* varAtom, std::unique_ptr<Data4tpl::RuleAtom<T>>& specAtom);
  
  /**
   * Helper function to convert one generic rule to the 
   * rule implementation used by five-tuple algorithms.
   *
   * @param varRule generic rule structure
   * @param specRule reference to five-tuple rule object
   */
  static void convertRule(const Generic::Rule& varRule, std::unique_ptr<Data4tpl::Rule>& specRule);

  /**
   * Helper function to convert a generic header line to the
   * implementation used by five-tuple algorithms.
   *
   * @param line generic header data line
   * @param tuple reference to already created header tuple-object which will be filled
   */
	static void convertHeader(const Generic::PacketHeaderLine& line, Data4tpl::HeaderTuple& tuple);
};

#endif

