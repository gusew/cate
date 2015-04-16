#include <algorithms/Converter10tpl.hpp>
#include <cstdint>

template <>
void Converter10tpl::convertRuleAtom<uint32_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data10tpl::RuleAtom<uint32_t>>& specAtom) {
  // get addr atom
  switch(varAtom->getType()) {

  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_uint_p()) 
      throw "Converter10tpl: Conversion failed, exact rule atom value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomExact<uint32_t>((uint32_t)exact->value.get_ui()));
    break; 
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_uint_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom min-value exceeds expected size (32 bits).";
    if (!range->maxValue.fits_uint_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom max-value exceeds expected size (32 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomRange<uint32_t>((uint32_t)range->minValue.get_ui(), (uint32_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_uint_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom prefix exceeds expected size (32 bits).";
    if (!prefix->mask.fits_uint_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom mask exceeds expected size (32 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomPrefix<uint32_t>((uint32_t)prefix->prefix.get_ui(), (uint32_t)prefix->mask.get_ui()));
    break;
  }

  } // switch
}

template <>
void Converter10tpl::convertRuleAtom<uint16_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data10tpl::RuleAtom<uint16_t>>& specAtom) {
  // get port atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, exact rule atom value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomExact<uint16_t>((uint16_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom min-value exceeds expected size (16 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom max-value exceeds expected size (16 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomRange<uint16_t>((uint16_t)range->minValue.get_ui(), (uint16_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom prefix exceeds expected size (16 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom mask exceeds expected size (16 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomPrefix<uint16_t>((uint16_t)prefix->prefix.get_ui(), (uint16_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

template <>
void Converter10tpl::convertRuleAtom<uint8_t>(Generic::RuleAtom* varAtom, std::unique_ptr<Data10tpl::RuleAtom<uint8_t>>& specAtom) {
  // get protocol or version atom
  switch(varAtom->getType()) {
  
  case Generic::RuleAtom::Type::EXACT: {
    Generic::RuleAtomExact* exact = static_cast<Generic::RuleAtomExact*>(varAtom);
    
    if (!exact->value.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, exact rule atom value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomExact<uint8_t>((uint8_t)exact->value.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::RANGE: {
    Generic::RuleAtomRange* range = static_cast<Generic::RuleAtomRange*>(varAtom);
    
    if (!range->minValue.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom min-value exceeds expected size (8 bits).";
    if (!range->maxValue.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, range rule atom max-value exceeds expected size (8 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomRange<uint8_t>((uint8_t)range->minValue.get_ui(), (uint8_t)range->maxValue.get_ui()));
    break;
  }

  case Generic::RuleAtom::Type::PREFIX: {
    Generic::RuleAtomPrefix* prefix = static_cast<Generic::RuleAtomPrefix*>(varAtom);
    
    if (!prefix->prefix.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom prefix exceeds expected size (8 bits).";
    if (!prefix->mask.fits_ushort_p()) 
      throw "Converter10tpl: Conversion failed, prefix rule atom mask exceeds expected size (8 bits).";
    
    specAtom.reset(new Data10tpl::RuleAtomPrefix<uint8_t>((uint8_t)prefix->prefix.get_ui(), (uint8_t)prefix->mask.get_ui()));
    break;
  }
  
  } // switch
}

void Converter10tpl::convertRule(const Generic::Rule& varRule, std::unique_ptr<Data10tpl::Rule>& specRule) {
  if (varRule.size() < 10) 
    throw "A rule inside the generic ruleset has not enough rule-atoms for ten-tuple compatibility.";

  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch1; 
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch2;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch3;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch4;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch5;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch6;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch7;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch8;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch9;
  std::unique_ptr<Data10tpl::RuleAtom<uint32_t>> ch10;

  Generic::RuleAtom* c1 = varRule[0].get();
  Generic::RuleAtom* c2 = varRule[1].get();
  Generic::RuleAtom* c3 = varRule[2].get();
  Generic::RuleAtom* c4 = varRule[3].get();
  Generic::RuleAtom* c5 = varRule[4].get();
  Generic::RuleAtom* c6 = varRule[5].get();
  Generic::RuleAtom* c7 = varRule[6].get();
  Generic::RuleAtom* c8 = varRule[7].get();
  Generic::RuleAtom* c9 = varRule[8].get();
  Generic::RuleAtom* c10 = varRule[9].get();

  convertRuleAtom<uint32_t>(c1, ch1);
  convertRuleAtom<uint32_t>(c2, ch2);  
  convertRuleAtom<uint32_t>(c3, ch3);  
  convertRuleAtom<uint32_t>(c4, ch4);  
  convertRuleAtom<uint32_t>(c5, ch5);  
  convertRuleAtom<uint32_t>(c6, ch6);  
  convertRuleAtom<uint32_t>(c7, ch7);  
  convertRuleAtom<uint32_t>(c8, ch8);  
  convertRuleAtom<uint32_t>(c9, ch9);  
  convertRuleAtom<uint32_t>(c10, ch10);  

  specRule.reset(new Data10tpl::Rule( 
    std::move(ch1), 
    std::move(ch2), 
    std::move(ch3), 
    std::move(ch4), 
    std::move(ch5),
    std::move(ch6), 
    std::move(ch7), 
    std::move(ch8), 
    std::move(ch9), 
    std::move(ch10)
  ));
}

void Converter10tpl::convertHeader(const Generic::PacketHeaderLine& line, Data10tpl::HeaderTuple& tuple) {
  if (line.size() < 10) 
    throw "Header data line has insufficent amount of values (required are 10).";
  if (!line[0]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v1 exceeds expected size.";
  if (!line[1]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v2 exceeds expected size.";
  if (!line[2]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v3 exceeds expected size.";
  if (!line[3]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v4 exceeds expected size.";
  if (!line[4]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v5 exceeds expected size.";
  if (!line[5]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v6 exceeds expected size.";
  if (!line[6]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v7 exceeds expected size.";
  if (!line[7]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v8 exceeds expected size.";
  if (!line[8]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v9 exceeds expected size.";
  if (!line[9]->value.fits_uint_p()) 
    throw "Converter10tpl: Conversion of header failed: v10 exceeds expected size.";

  tuple.v1 = (uint32_t)line[0]->value.get_ui();
  tuple.v2 = (uint32_t)line[1]->value.get_ui();
  tuple.v3 = (uint32_t)line[2]->value.get_ui();
  tuple.v4 = (uint32_t)line[3]->value.get_ui();
  tuple.v5 = (uint32_t)line[4]->value.get_ui();
  tuple.v6 = (uint32_t)line[5]->value.get_ui();
  tuple.v7 = (uint32_t)line[6]->value.get_ui();
  tuple.v8 = (uint32_t)line[7]->value.get_ui();
  tuple.v9 = (uint32_t)line[8]->value.get_ui();
  tuple.v10 = (uint32_t)line[9]->value.get_ui();
}

