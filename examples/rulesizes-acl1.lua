algLinSearch = createAlgorithm("../algorithms/lib/LinearSearch5tpl.so", {10000})
algBitvector = createAlgorithm("../algorithms/lib/Bitvector5tpl.so", {10000})
algTuples = createAlgorithm("../algorithms/lib/TupleSpace5tpl.so", {10000, 107})
algHiCuts = createAlgorithm("../algorithms/lib/HiCuts5tpl.so", {10000, 36, 3.0})

structureIPv4 = {32, 32, 16, 16, 8}

dofile("examples/rules/rls_5tpl-acl1-200.lua")
dofile("examples/rules/rls_5tpl-acl1-1000.lua")
dofile("examples/rules/rls_5tpl-acl1-2000.lua")
dofile("examples/rules/rls_5tpl-acl1-5000.lua")

dofile("examples/headers/hdr_5tpl-acl1-200.lua")
dofile("examples/headers/hdr_5tpl-acl1-1000.lua")
dofile("examples/headers/hdr_5tpl-acl1-2000.lua")
dofile("examples/headers/hdr_5tpl-acl1-5000.lua")

registerBenchmark("5-tuple with acl1 classifier", algLinSearch, structureIPv4, rls_5tpl_acl1_200, hdr_5tpl_acl1_200, 8)
registerBenchmark("5-tuple with acl1 classifier", algLinSearch, structureIPv4, rls_5tpl_acl1_1000, hdr_5tpl_acl1_1000, 8)
registerBenchmark("5-tuple with acl1 classifier", algLinSearch, structureIPv4, rls_5tpl_acl1_2000, hdr_5tpl_acl1_2000, 8)
registerBenchmark("5-tuple with acl1 classifier", algLinSearch, structureIPv4, rls_5tpl_acl1_5000, hdr_5tpl_acl1_5000, 8)

registerBenchmark("5-tuple with acl1 classifier", algBitvector, structureIPv4, rls_5tpl_acl1_200, hdr_5tpl_acl1_200, 8)
registerBenchmark("5-tuple with acl1 classifier", algBitvector, structureIPv4, rls_5tpl_acl1_1000, hdr_5tpl_acl1_1000, 8)
registerBenchmark("5-tuple with acl1 classifier", algBitvector, structureIPv4, rls_5tpl_acl1_2000, hdr_5tpl_acl1_2000, 8)
registerBenchmark("5-tuple with acl1 classifier", algBitvector, structureIPv4, rls_5tpl_acl1_5000, hdr_5tpl_acl1_5000, 8)

registerBenchmark("5-tuple with acl1 classifier", algTuples, structureIPv4, rls_5tpl_acl1_200, hdr_5tpl_acl1_200, 8)
registerBenchmark("5-tuple with acl1 classifier", algTuples, structureIPv4, rls_5tpl_acl1_1000, hdr_5tpl_acl1_1000, 8)
registerBenchmark("5-tuple with acl1 classifier", algTuples, structureIPv4, rls_5tpl_acl1_2000, hdr_5tpl_acl1_2000, 8)
registerBenchmark("5-tuple with acl1 classifier", algTuples, structureIPv4, rls_5tpl_acl1_5000, hdr_5tpl_acl1_5000, 8)

registerBenchmark("5-tuple with acl1 classifier", algHiCuts, structureIPv4, rls_5tpl_acl1_200, hdr_5tpl_acl1_200, 8)
registerBenchmark("5-tuple with acl1 classifier", algHiCuts, structureIPv4, rls_5tpl_acl1_1000, hdr_5tpl_acl1_1000, 8)
registerBenchmark("5-tuple with acl1 classifier", algHiCuts, structureIPv4, rls_5tpl_acl1_2000, hdr_5tpl_acl1_2000, 8)
registerBenchmark("5-tuple with acl1 classifier", algHiCuts, structureIPv4, rls_5tpl_acl1_5000, hdr_5tpl_acl1_5000, 8)

