#include <algorithms/Converter2tpl.hpp>
#include <cstdint>

template <>
void Converter2tpl::convertRuleAtom<uint32_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data2tpl::RuleAtom<uint32_t>>& specAtom) {
  // get addr atom
  switch(varAtom->getType()) {

  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_uint_p()) 
      throw "Converter2tpl: Conversion failed, exact rule atom value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomExact<uint32_t>((uint32_t)exact->value.get_ui()));
    break; 
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_uint_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom min-value exceeds expected size (32 bits).";
    if (!range->maxValue.fits_uint_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom max-value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomRange<uint32_t>((uint32_t)range->minValue.get_ui(), (uint32_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_uint_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom prefix exceeds expected size (32 bits).";
    if (!prefix->mask.fits_uint_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom mask exceeds expected size (32 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomPrefix<uint32_t>((uint32_t)prefix->prefix.get_ui(), (uint32_t)prefix->mask.get_ui()));
    break;
  }

  } // switch
}

template <>
void Converter2tpl::convertRuleAtom<uint16_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data2tpl::RuleAtom<uint16_t>>& specAtom) {
  // get port atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, exact rule atom value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomExact<uint16_t>((uint16_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom min-value exceeds expected size (16 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom max-value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomRange<uint16_t>((uint16_t)range->minValue.get_ui(), (uint16_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom prefix exceeds expected size (16 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom mask exceeds expected size (16 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomPrefix<uint16_t>((uint16_t)prefix->prefix.get_ui(), (uint16_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

template <>
void Converter2tpl::convertRuleAtom<uint8_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data2tpl::RuleAtom<uint8_t>>& specAtom) {
  // get protocol or version atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, exact rule atom value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomExact<uint8_t>((uint8_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom min-value exceeds expected size (8 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, range rule atom max-value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomRange<uint8_t>((uint8_t)range->minValue.get_ui(), (uint8_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom prefix exceeds expected size (8 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter2tpl: Conversion failed, prefix rule atom mask exceeds expected size (8 bits).";
    
    specAtom.reset(new Data2tpl::RuleAtomPrefix<uint8_t>((uint8_t)prefix->prefix.get_ui(), (uint8_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

void Converter2tpl::convertRule(const Generic::Rule& varRule, std::unique_ptr<Data2tpl::Rule>& specRule) {
  if (varRule.size() < 2) 
    throw "A rule inside the generic ruleset has not enough rule-atoms for two-tuple compatibility.";

  std::unique_ptr<Data2tpl::RuleAtom<uint32_t>> addrSrc;
  std::unique_ptr<Data2tpl::RuleAtom<uint32_t>> addrDest;

  Generic::RuleAtom* addrSrcGeneric = varRule[0].get();
  Generic::RuleAtom* addrDestGeneric = varRule[1].get();

  convertRuleAtom<uint32_t>(addrSrcGeneric, addrSrc);  
  convertRuleAtom<uint32_t>(addrDestGeneric, addrDest);  

  specRule.reset(new Data2tpl::Rule( 
    std::move(addrSrc), 
    std::move(addrDest) 
  ));
}

void Converter2tpl::convertHeader(const Generic::PacketHeaderLine& line, Data2tpl::HeaderTuple& tuple) {
  if (line.size() < 2) 
    throw "Header data line has insufficent amount of values (required are 2).";
  if (!line[0]->value.fits_uint_p()) 
    throw "Converter2tpl: Conversion of header failed: ip src exceeds expected size.";
  if (!line[1]->value.fits_uint_p()) 
    throw "Converter2tpl: Conversion of header failed: ip dst exceeds expected size.";

  tuple.addrSrc = (uint32_t)line[0]->value.get_ui();
  tuple.addrDest = (uint32_t)line[1]->value.get_ui();
}
