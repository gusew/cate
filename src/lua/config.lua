--[[
	This is a user-script for defining a benchmark-configuration with lua. You can
	(and should) use some predefined helper functions which provide a correct
	data handling to CATE. These are:
		createAlgorithm(<filename>, <params>)
		createRuleset()
			addRuleToRuleset(<ruleset>, <rule>)
			ruleAtomExact(<value>)
			ruleAtomRange(<min_value>, <max_value>)
			ruleAtomPrefix(<prefix_value>, <mask>)
		ipv4Toi(ip)
		maskToi(<maskbits>, [<totalBits>]) (default: totalBits=32)
		createHeaderset()
			addHeaderToHeaderset(<headerset>, <header>)
		createRandomHeaders(<amount>, <output headers>, <distributions>)
			constantDistribution(<value>)
			uniformDistribution(<seed>, <min>, <max>)
			normalDistribution(<seed>, <mean>, <stddev>)
			lognormalDistribution(<seed>, <m>, <s>)
			exponentialDistribution(<seed>, <lambda>)
			cauchyDistribution(<seed>, <a>, <b>)
			paretoDistribution(<seed>, <scale>, <shape>, <offset>)
		registerBenchmark(<caption_text>, <algorithm>, <structure>, <rules>, <headers>, <amount_runs>)
]]

-- Specify some classification algorithms
algLinSearch = createAlgorithm("LinearSearch5tpl.so", {100})
algBitvector = createAlgorithm("Bitvector5tpl.so", {100})
algHiCuts = createAlgorithm("HiCuts5tpl.so", {100, 16, 3.0})
algTuples = createAlgorithm("TupleSpace5tpl.so", {1000, 797})

-- Definition of header data structure (amount of bits for each field)
structureIPv4 = {32, 32, 16, 16, 8} -- ip, ip, port, port, protocol
structureIPv6 = {128, 128, 16, 16, 8} -- ip, ip, port, port, protocol

-- Specify an IPv4 ruleset
rs = createRuleset()
addRuleToRuleset(rs, {
	ruleAtomExact(ipv4Toi("10.0.0.20")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.0.1"), maskToi(16)), -- ipDest
	ruleAtomPrefix(0, maskToi(0)), -- portSrc
	ruleAtomRange(2000, 4000), -- portDest
	ruleAtomExact(6)	-- protocol
})
addRuleToRuleset(rs, { 
	ruleAtomRange(ipv4Toi("10.0.0.18"), ipv4Toi("10.0.0.22")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.100.0"), maskToi(24, 32)), -- ipDest
	ruleAtomPrefix(0, maskToi(0)), -- portSrc
	ruleAtomRange(1, 0xFFFF), -- portDest
	ruleAtomExact(6)	-- protocol
})
addRuleToRuleset(rs, { 
	ruleAtomRange(ipv4Toi("10.0.0.18"), ipv4Toi("10.0.0.22")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.100.0"), maskToi(24, 32)), -- ipDest
	ruleAtomPrefix(0, maskToi(0)), -- portSrc
	ruleAtomRange(32, 1024), -- portDest
	ruleAtomExact(6)	-- protocol
})
addRuleToRuleset(rs, { 
	ruleAtomRange(ipv4Toi("10.0.0.18"), ipv4Toi("10.0.0.22")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.100.0"), maskToi(24, 32)), -- ipDest
	ruleAtomExact(443), -- portSrc
	ruleAtomExact(80), -- portDest
	ruleAtomExact(9)	-- protocol
})
addRuleToRuleset(rs, { 
	ruleAtomExact(ipv4Toi("10.0.0.20")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.100.0"), maskToi(24, 32)), -- ipDest
	ruleAtomExact(800), -- portSrc
	ruleAtomRange(1, 0xFFFF), -- portDest
	ruleAtomExact(6)	-- protocol
})
addRuleToRuleset(rs, { 
	ruleAtomRange(0, ipv4Toi("255.255.255.255")), -- ipSrc
	ruleAtomPrefix(ipv4Toi("10.10.100.0"), maskToi(0, 32)), -- ipDest
	ruleAtomPrefix(0, maskToi(0)), -- portSrc
	ruleAtomRange(1, 0xFFFF), -- portDest
	ruleAtomRange(0, 255)	-- protocol
})

-- Specify exact headers for IPv4 
headers1 = createHeaderset()
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {815849182, 2147483647, 161, 0, 17})
addHeaderToHeaderset(headers1, {1538872396, 2147483647, 65535, 0, 47})
addHeaderToHeaderset(headers1, {2147483647, 1421352615, 65535, 53, 17})
addHeaderToHeaderset(headers1, {1543503872, 1421352611, 65535, 22, 17})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1538872391, 126670924, 65535, 65535, 1})
addHeaderToHeaderset(headers1, {1509949439, 1421352616, 0, 7648, 17})
addHeaderToHeaderset(headers1, {1509949439, 1421352616, 0, 7648, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337940, 1024, 67, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337940, 1024, 67, 17})
addHeaderToHeaderset(headers1, {1073741824, 0, 65535, 161, 17})
addHeaderToHeaderset(headers1, {1073741824, 0, 65535, 161, 17})
addHeaderToHeaderset(headers1, {1073741824, 0, 65535, 161, 17})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1342177279, 1421352612, 65535, 25, 6})
addHeaderToHeaderset(headers1, {1538872394, 1073741823, 750, 0, 17})
addHeaderToHeaderset(headers1, {1538872394, 1073741823, 750, 0, 17})
addHeaderToHeaderset(headers1, {1538872394, 1073741823, 750, 0, 17})
addHeaderToHeaderset(headers1, {4294967295, 1421352608, 750, 65535, 17})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1073741824, 126674048, 0, 161, 6})
addHeaderToHeaderset(headers1, {1610612736, 126670924, 65535, 544, 17})
addHeaderToHeaderset(headers1, {1610612736, 126670924, 65535, 544, 17})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1610612736, 4294967295, 0, 65535, 6})
addHeaderToHeaderset(headers1, {1538872831, 1073741824, 65535, 0, 1})
addHeaderToHeaderset(headers1, {1538872393, 2147483647, 53, 65535, 17})
addHeaderToHeaderset(headers1, {1538872393, 2147483647, 53, 65535, 17})
addHeaderToHeaderset(headers1, {1538872393, 2147483647, 53, 65535, 17})
addHeaderToHeaderset(headers1, {1538872399, 4294967295, 65535, 25, 6})
addHeaderToHeaderset(headers1, {1538872399, 4294967295, 65535, 25, 6})
addHeaderToHeaderset(headers1, {1538872399, 1409286143, 0, 0, 6})
addHeaderToHeaderset(headers1, {1538872393, 0, 65535, 25, 6})
addHeaderToHeaderset(headers1, {1538872393, 0, 65535, 25, 6})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {2147483647, 4257337935, 1024, 65535, 17})
addHeaderToHeaderset(headers1, {1538916351, 126746624, 65535, 22, 6})
addHeaderToHeaderset(headers1, {1538916351, 126746624, 65535, 22, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {4294967295, 1421352616, 0, 69, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1073741824, 1421352613, 0, 80, 6})
addHeaderToHeaderset(headers1, {1342177279, 1421352612, 65535, 25, 6}

-- Specify random header generation for IPv4 
headersRnd = createRandomHeaders(1000, true, {
	uniformDistribution(1111, ipv4Toi("10.0.0.18"), ipv4Toi("10.0.0.22")),
	uniformDistribution(2222, ipv4Toi("10.10.0.1"), ipv4Toi("10.10.255.255")),
	uniformDistribution(3333, 1, 32000),
	uniformDistribution(4444, 1024, 8048),
	constantDistribution(6)
})

-- Define tests
registerBenchmark("Linear Search (5-tpl)", algLinSearch, structureIPv4, rs, headersRnd, 8)
registerBenchmark("Bitvector (5-tpl)", algBitvector, structureIPv4, rs, headersRnd, 8)
registerBenchmark("HiCuts (5-tpl)", algHiCuts, structureIPv4, rs, headersRnd, 8)
registerBenchmark("Tuple Space (5-tpl)", algTuples, structureIPv4, rs, headersRnd, 8)

