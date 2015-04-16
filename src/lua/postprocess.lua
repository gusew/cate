--[[
	This file will be concatenated to the user configuration file. It provides 
	validity and sanity checks before returning the benchmark suite object to
	the caller (CATE-program).
]]

if (_CATE_benchmarksuite == nil) then 
	error("Internal variable '_CATE_benchmarksuite' does not exist.") 
end
if (#_CATE_benchmarksuite == 0) then
	error("No benchmarks were specified. Nothing to do.")
end

-- returns max. numerical value for a given amount of bits
function _CATE_maxValueForBits(bits)
	local maxVal = 0
	for i = 0, bits-1 do
		maxVal = maxVal + 2^i	
	end
	return maxVal
end

-- checks, if a (numerical) value can be represented in given amount of bits
function _CATE_checkValueSize(value, bits)
	if (value < 0 or value > _CATE_maxValueForBits(bits)) then
		error("Specified value ("..value..") exceeds size specified by "..bits.." bits.", 2) 
	end
end

-- checks, if rules follow specified structure
function _CATE_checkRules(rules, structure)
	for i = 1, #rules do -- iterate over each rule
		if (#rules[i] ~= #structure) then 
			error("Validity error! Amount of rule atoms does not match header data structure.") 
		end

		for j = 1, #rules[i] do -- iterate over each rule atom
			if (#rules[i][j] < 2) then error("Missing parameters for rule atom definition.") end

			if (rules[i][j][1] == 0) then -- exact value
				_CATE_checkValueSize(rules[i][j][2], structure[j])
			elseif (rules[i][j][1] == 1 or rules[i][j][1] == 2) then -- range or prefix
				_CATE_checkValueSize(rules[i][j][2], structure[j])
				_CATE_checkValueSize(rules[i][j][3], structure[j])
			else error("Rule atom is of invalid type ("..rules[i][j][1]..").") end
		end 
	end
end

-- checks, if configuration for header generation follow structure
function _CATE_checkHeadersRandom(distributions, structure)
	if (#distributions ~= #structure) then
		error("Validity error! Amount of random distributions for header generation does not match header structure.")
	end

	for i=1, #distributions do -- iterate over each distribution
		if (distributions[i][1] == 0) then
			_CATE_checkValueSize(distributions[i][2], structure[i])
		elseif (distributions[i][1] == 1) then
			_CATE_checkValueSize(distributions[i][3], structure[i])
			_CATE_checkValueSize(distributions[i][4], structure[i])
		end
	end
end

-- checks, if headers follow specified structure
function _CATE_checkHeaders(headers, structure)
	for i = 1, #headers do -- iterate over each header
		if (#headers[i] ~= #structure) then
			error("Validity error! Amount of header data elements does not match header structure.")
		end

		for j = 1, #headers[i] do -- iterate over each header element
			_CATE_checkValueSize(headers[i][j], structure[j])
		end
	end
end

-- checks, if given amount of repetitions is reasonable
function _CATE_checkRepetitions(repetitions)
	if (repetitions <= 0) then
		error("Validity error! Amount of runs must be at least 1.")
	end
end

-- delegates calls to check each element of a benchmark
function _CATE_checkBenchmark(benchmark)
	_CATE_checkRules(benchmark[4], benchmark[3])

	-- check if headers are specified explicit or for random generation
	if (benchmark[5][1] == "random") then
		if (benchmark[5][2] < 0) then
			error("Validity error! A negative amount of headers to generate was specified.")
		elseif (benchmark[5][3] ~= true and benchmark[5][3] ~= false) then
			error("No valid configuration for output of headers to file given (expected was 'true' or 'false').")
		else
			_CATE_checkHeadersRandom(benchmark[5][4], benchmark[3])
		end
	elseif (benchmark[5][1] == "exact") then
		_CATE_checkHeaders(benchmark[5][2], benchmark[3])
	else
		error("Headers were specified in an unknown format.")
	end

	_CATE_checkRepetitions(benchmark[6])
end

for i = 1, #_CATE_benchmarksuite do
	_CATE_checkBenchmark(_CATE_benchmarksuite[i])
end

-- rien ne va plus
print("CATE configuration was successfully validated.")
return _CATE_benchmarksuite

