#include <algorithms/common/Converter4tpl.hpp>
#include <cstdint>

template <>
void Converter4tpl::convertRuleAtom<uint32_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data4tpl::RuleAtom<uint32_t>>& specAtom) {
  // get addr atom
  switch(varAtom->getType()) {

  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_uint_p()) 
      throw "Converter4tpl: Conversion failed, exact rule atom value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomExact<uint32_t>((uint32_t)exact->value.get_ui()));
    break; 
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_uint_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom min-value exceeds expected size (32 bits).";
    if (!range->maxValue.fits_uint_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom max-value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomRange<uint32_t>((uint32_t)range->minValue.get_ui(), (uint32_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_uint_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom prefix exceeds expected size (32 bits).";
    if (!prefix->mask.fits_uint_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom mask exceeds expected size (32 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomPrefix<uint32_t>((uint32_t)prefix->prefix.get_ui(), (uint32_t)prefix->mask.get_ui()));
    break;
  }

  } // switch
}

template <>
void Converter4tpl::convertRuleAtom<uint16_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data4tpl::RuleAtom<uint16_t>>& specAtom) {
  // get port atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, exact rule atom value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomExact<uint16_t>((uint16_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom min-value exceeds expected size (16 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom max-value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomRange<uint16_t>((uint16_t)range->minValue.get_ui(), (uint16_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom prefix exceeds expected size (16 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom mask exceeds expected size (16 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomPrefix<uint16_t>((uint16_t)prefix->prefix.get_ui(), (uint16_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

template <>
void Converter4tpl::convertRuleAtom<uint8_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data4tpl::RuleAtom<uint8_t>>& specAtom) {
  // get protocol or version atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, exact rule atom value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomExact<uint8_t>((uint8_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom min-value exceeds expected size (8 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, range rule atom max-value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomRange<uint8_t>((uint8_t)range->minValue.get_ui(), (uint8_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom prefix exceeds expected size (8 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter4tpl: Conversion failed, prefix rule atom mask exceeds expected size (8 bits).";
    
    specAtom.reset(new Data4tpl::RuleAtomPrefix<uint8_t>((uint8_t)prefix->prefix.get_ui(), (uint8_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

void Converter4tpl::convertRule(const Generic::Rule& varRule, std::unique_ptr<Data4tpl::Rule>& specRule) {
  if (varRule.size() < 4) 
    throw "A rule inside the generic ruleset has not enough rule-atoms for 4-tuple compatibility.";

  std::unique_ptr<Data4tpl::RuleAtom<uint32_t>> ch1;
  std::unique_ptr<Data4tpl::RuleAtom<uint32_t>> ch2;
  std::unique_ptr<Data4tpl::RuleAtom<uint32_t>> ch3;
  std::unique_ptr<Data4tpl::RuleAtom<uint32_t>> ch4;

  Generic::RuleAtom* ch1Generic = varRule[0].get();
  Generic::RuleAtom* ch2Generic = varRule[1].get();
  Generic::RuleAtom* ch3Generic = varRule[2].get();
  Generic::RuleAtom* ch4Generic = varRule[3].get();

  convertRuleAtom<uint32_t>(ch1Generic, ch1);  
  convertRuleAtom<uint32_t>(ch2Generic, ch2);  
  convertRuleAtom<uint32_t>(ch3Generic, ch3);  
  convertRuleAtom<uint32_t>(ch4Generic, ch4);  

  specRule.reset(new Data4tpl::Rule( 
    std::move(ch1), 
    std::move(ch2), 
    std::move(ch3), 
    std::move(ch4) 
  ));
}

void Converter4tpl::convertHeader(const Generic::PacketHeaderLine& line, Data4tpl::HeaderTuple& tuple) {
  if (line.size() < 4) 
    throw "Header data line has insufficent amount of values (required are 4).";
  if (!line[0]->value.fits_uint_p()) 
    throw "Converter4tpl: Conversion of header failed: val 1 exceeds expected size.";
  if (!line[1]->value.fits_uint_p()) 
    throw "Converter4tpl: Conversion of header failed: val 2 exceeds expected size.";
  if (!line[2]->value.fits_uint_p()) 
    throw "Converter4tpl: Conversion of header failed: val 3 exceeds expected size.";
  if (!line[3]->value.fits_uint_p()) 
    throw "Converter4tpl: Conversion of header failed: val 4 exceeds expected size.";

  tuple.v1 = (uint32_t)line[0]->value.get_ui();
  tuple.v2 = (uint32_t)line[1]->value.get_ui();
  tuple.v3 = (uint32_t)line[2]->value.get_ui();
  tuple.v4 = (uint32_t)line[3]->value.get_ui();
}
