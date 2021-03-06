algLinSearch2 = createAlgorithm("../algorithms/lib/LinearSearch2tpl.so", {10000})
algLinSearch4 = createAlgorithm("../algorithms/lib/LinearSearch4tpl.so", {10000})
algLinSearch10 = createAlgorithm("../algorithms/lib/LinearSearch10tpl.so", {10000})

algBitvector2 = createAlgorithm("../algorithms/lib/Bitvector2tpl.so", {10000})
algBitvector4 = createAlgorithm("../algorithms/lib/Bitvector4tpl.so", {10000})
algBitvector10 = createAlgorithm("../algorithms/lib/Bitvector10tpl.so", {10000})

algTuples2 = createAlgorithm("../algorithms/lib/TupleSpace2tpl.so", {10000, 107})
algTuples4 = createAlgorithm("../algorithms/lib/TupleSpace4tpl.so", {10000, 107})
algTuples10 = createAlgorithm("../algorithms/lib/TupleSpace10tpl.so", {10000, 107})

algHiCuts2 = createAlgorithm("../algorithms/lib/HiCuts2tpl.so", {10000, 20, 3.0})
algHiCuts4 = createAlgorithm("../algorithms/lib/HiCuts4tpl.so", {10000, 20, 3.0})
algHiCuts10 = createAlgorithm("../algorithms/lib/HiCuts10tpl.so", {10000, 20, 3.0})


structure2 = {32, 32}
structure4 = {32, 32, 32, 32}
structure10 = {32, 32, 32, 32, 32, 32, 32, 32, 32, 32}

dofile("examples/rules/rls_2tpl-fw1-1000.lua")
dofile("examples/rules/rls_4tpl-fw1-1000.lua")
dofile("examples/rules/rls_10tpl-fw1-1000.lua")

dofile("examples/headers/hdr_2tpl-fw1-1000.lua")
dofile("examples/headers/hdr_4tpl-fw1-1000.lua")
dofile("examples/headers/hdr_10tpl-fw1-1000.lua")

registerBenchmark("2-tuple with fw1 classifier", algLinSearch2, structure2, rls_2tpl_fw1_1000, hdr_2tpl_fw1_1000, 8)
registerBenchmark("4-tuple with fw1 classifier", algLinSearch4, structure4, rls_4tpl_fw1_1000, hdr_4tpl_fw1_1000, 8)
registerBenchmark("10-tuple with fw1 classifier", algLinSearch10, structure10, rls_10tpl_fw1_1000, hdr_10tpl_fw1_1000, 8)

registerBenchmark("2-tuple with fw1 classifier", algBitvector2, structure2, rls_2tpl_fw1_1000, hdr_2tpl_fw1_1000, 8)
registerBenchmark("4-tuple with fw1 classifier", algBitvector4, structure4, rls_4tpl_fw1_1000, hdr_4tpl_fw1_1000, 8)
registerBenchmark("10-tuple with fw1 classifier", algBitvector10, structure10, rls_10tpl_fw1_1000, hdr_10tpl_fw1_1000, 8)

registerBenchmark("2-tuple with fw1 classifier", algTuples2, structure2, rls_2tpl_fw1_1000, hdr_2tpl_fw1_1000, 8)
registerBenchmark("4-tuple with fw1 classifier", algTuples4, structure4, rls_4tpl_fw1_1000, hdr_4tpl_fw1_1000, 8)
registerBenchmark("10-tuple with fw1 classifier", algTuples10, structure10, rls_10tpl_fw1_1000, hdr_10tpl_fw1_1000, 8)

registerBenchmark("2-tuple with fw1 classifier", algHiCuts2, structure2, rls_2tpl_fw1_1000, hdr_2tpl_fw1_1000, 8)
registerBenchmark("4-tuple with fw1 classifier", algHiCuts4, structure4, rls_4tpl_fw1_1000, hdr_4tpl_fw1_1000, 8)
registerBenchmark("10-tuple with fw1 classifier", algHiCuts10, structure10, rls_10tpl_fw1_1000, hdr_10tpl_fw1_1000, 8)

