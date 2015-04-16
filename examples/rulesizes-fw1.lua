algLinSearch = createAlgorithm("LinearSearch5tpl.so", {10000})
algBitvector = createAlgorithm("Bitvector5tpl.so", {10000})
algTuples = createAlgorithm("TupleSpace5tpl.so", {10000, 107})
algHiCuts = createAlgorithm("HiCuts5tpl.so", {10000, 36, 3.0})

structureIPv4 = {32, 32, 16, 16, 8}

dofile("examples/rules/rls_5tpl-fw1-200.lua")
dofile("examples/rules/rls_5tpl-fw1-1000.lua")
dofile("examples/rules/rls_5tpl-fw1-2000.lua")
dofile("examples/rules/rls_5tpl-fw1-5000.lua")

dofile("examples/headers/hdr_5tpl-fw1-200.lua")
dofile("examples/headers/hdr_5tpl-fw1-1000.lua")
dofile("examples/headers/hdr_5tpl-fw1-2000.lua")
dofile("examples/headers/hdr_5tpl-fw1-5000.lua")

registerBenchmark("5-tuple with fw1 classifier", algLinSearch, structureIPv4, rls_5tpl_fw1_200, hdr_5tpl_fw1_200, 8)
registerBenchmark("5-tuple with fw1 classifier", algLinSearch, structureIPv4, rls_5tpl_fw1_1000, hdr_5tpl_fw1_1000, 8)
registerBenchmark("5-tuple with fw1 classifier", algLinSearch, structureIPv4, rls_5tpl_fw1_2000, hdr_5tpl_fw1_2000, 8)
registerBenchmark("5-tuple with fw1 classifier", algLinSearch, structureIPv4, rls_5tpl_fw1_5000, hdr_5tpl_fw1_5000, 8)

registerBenchmark("5-tuple with fw1 classifier", algBitvector, structureIPv4, rls_5tpl_fw1_200, hdr_5tpl_fw1_200, 8)
registerBenchmark("5-tuple with fw1 classifier", algBitvector, structureIPv4, rls_5tpl_fw1_1000, hdr_5tpl_fw1_1000, 8)
registerBenchmark("5-tuple with fw1 classifier", algBitvector, structureIPv4, rls_5tpl_fw1_2000, hdr_5tpl_fw1_2000, 8)
registerBenchmark("5-tuple with fw1 classifier", algBitvector, structureIPv4, rls_5tpl_fw1_5000, hdr_5tpl_fw1_5000, 8)

registerBenchmark("5-tuple with fw1 classifier", algTuples, structureIPv4, rls_5tpl_fw1_200, hdr_5tpl_fw1_200, 8)
registerBenchmark("5-tuple with fw1 classifier", algTuples, structureIPv4, rls_5tpl_fw1_1000, hdr_5tpl_fw1_1000, 8)
registerBenchmark("5-tuple with fw1 classifier", algTuples, structureIPv4, rls_5tpl_fw1_2000, hdr_5tpl_fw1_2000, 8)
registerBenchmark("5-tuple with fw1 classifier", algTuples, structureIPv4, rls_5tpl_fw1_5000, hdr_5tpl_fw1_5000, 8)

registerBenchmark("5-tuple with fw1 classifier", algHiCuts, structureIPv4, rls_5tpl_fw1_200, hdr_5tpl_fw1_200, 8)
registerBenchmark("5-tuple with fw1 classifier", algHiCuts, structureIPv4, rls_5tpl_fw1_1000, hdr_5tpl_fw1_1000, 8)
registerBenchmark("5-tuple with fw1 classifier", algHiCuts, structureIPv4, rls_5tpl_fw1_2000, hdr_5tpl_fw1_2000, 8)
registerBenchmark("5-tuple with fw1 classifier", algHiCuts, structureIPv4, rls_5tpl_fw1_5000, hdr_5tpl_fw1_5000, 8)

