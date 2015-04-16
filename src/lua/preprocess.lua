--[[
	This file contains lua-helper functions which are included in a lua-user scriptfile
	before the usercode. A user typically uses these functions for the definition of a
	benchmark suite.

	Please notice that Lua currently only guarantees 32Bit unsigned integer values. For
	greater numbers string-representations should be used.
]]

-- table with all necessary benchmark information, is filled inside 'register'-funct.
local _CATE_benchmarksuite = {}

-- Helper function to convert ip-string to 32Bit-integer
function ipv4Toi(ip) 
	local resultArr = {}
	local i = 1
	for byte in string.gmatch(ip, "(%d+)[.]?") do
		byte = tonumber(byte)
		if (byte < 0 or byte > 255) then 
			error("Invalid byte in IP-string given ("..byte..") to 'ipv4Toi'.", 2)
		end
		resultArr[i] = byte
		i = i+1
	end
	
	if (i == 5) then 
		-- make one single number from array
		local result, j = resultArr[4], 3
		for i = 1, 3 do
			result = result + resultArr[i] * 256^j -- shift bytes accordingly
			j = j-1
		end
		return result
	else
		error("Invalid IP-string given to 'ipv4Toi'.", 2)
	end
end

-- Helper function to convert 32Bit-integer mask-representation to correct high-bits-mask
function maskToi(mask, totalBits)
	-- set default value for total bits
	if (totalBits == nil) then totalBits = 32 end

	-- validity check for input parameter
	if (mask < 0 or mask > totalBits) then 
		error("Invalid mask value given. Allowed are values between 0 and "..totalBits..".", 2) 
	end

	local j = totalBits-1
	local result = 0
	-- set bits for mask-result from high to low
	for i = 1, mask do 
		result = result + 2^j
		j = j-1
	end
	return result
end

-- Represents one rule atom of a specified type in a custom ruleset.
function ruleAtomExact(value) return {0, value} end
function ruleAtomRange(min, max) return {1, min, max} end
function ruleAtomPrefix(prefix, mask) return {2, prefix, mask} end

-- Helper functions for creation of benchmark-elements
function createAlgorithm(filename, paramlist) return {filename, paramlist} end

function createRuleset() return {} end
function addRuleToRuleset(ruleset, rule)
	local index = #ruleset + 1
	ruleset[index] = rule
	return ruleset
end

function createHeaderset() return {"exact", {}} end
function addHeaderToHeaderset(headerset, header)	
	local index = #headerset[2] + 1
	headerset[2][index] = header
	return headerset
end

function createRandomHeaders(amount, output, distributions) return {"random", amount, output, distributions} end
function constantDistribution(value) return {0, value} end
function uniformDistribution(seed, min, max) return {1, seed, min, max} end
function normalDistribution(seed, mean, stddev) return {2, seed, mean, stddev} end
function lognormalDistribution(seed, m, s) return {3, seed, m, s} end
function exponentialDistribution(seed, lambda) return {4, seed, lambda} end
function cauchyDistribution(seed, a, b) return {5, seed, b, a} end
function paretoDistribution(seed, scale, shape, offset) return {6, seed, scale, shape, offset} end

-- Add a benchmark run to the current benchmark suite
function registerBenchmark(caption, algorithm, structure, rules, headers, amount_runs)
	local index = #_CATE_benchmarksuite + 1
	_CATE_benchmarksuite[index] = {caption, algorithm, structure, rules, headers, amount_runs}
end

