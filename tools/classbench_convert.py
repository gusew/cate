import sys
import re

def parse_ip(out, addr, mask):
    if int(mask) == 32:
        out.write("ruleAtomExact(ipv4Toi(\"%s\")), " % addr)
    else:
        out.write("ruleAtomPrefix(ipv4Toi(\"%s\"), maskToi(%s)), " % (addr, mask))

def parse_port(out, minp, maxp):
    if int(minp) == int(maxp):
        out.write("ruleAtomExact(%s), " % minp)
    else:
        out.write("ruleAtomRange(%s, %s), " % (minp, maxp))

def parse_prot(prot):
    return int(prot.split("/")[0], 16)


def split_subnet(subnet):
    return subnet.split("/")


def write_header(out, binth):
    out.write("algLinSearch = createAlgorithm(\"LinearSearch5tpl.so\", {10000})\n")
    out.write("algBitvector = createAlgorithm(\"Bitvector5tpl.so\", {10000})\n")
    out.write("algTuples = createAlgorithm(\"TupleSpace5tpl.so\", {10000, 107})\n\n")
    out.write("algHiCuts = createAlgorithm(\"HiCuts5tpl.so\", {10000, %s, 3.0})\n" % binth)
    out.write("structureIPv4 = {32, 32, 16, 16, 8}\n\n")


def write_rules(out, rulefile):
    with open(rulefile, "r") as f:
        lines = f.readlines()
    out.write("rs = createRuleset()\n")
    for line in lines:
        line = line.strip()
        parts = re.split("[ \t]+", line)
        
        out.write("addRuleToRuleset(rs, {")
        src_addr, src_mask = split_subnet(parts[0][1:])
        parse_ip(out, src_addr, src_mask)
        dst_addr, dst_mask = split_subnet(parts[1])
        parse_ip(out, dst_addr, dst_mask)
        parse_port(out, parts[2], parts[4])
        parse_port(out, parts[5], parts[7])
        out.write("ruleAtomExact(%d)" % parse_prot(parts[8]))
        out.write("})\n")
    out.write("\n")


def write_trace(out, tracefile):
    with open(tracefile, "r") as f:
        lines = f.readlines()
    out.write("\n\n")
    out.write("headers = createHeaderset()\n")
    for line in lines:
        line = line.strip()
        parts = re.split("[ \t]+", line)
        out.write("addHeaderToHeaderset(headers, {")
        out.write("%s, %s, %s, %s, %s})\n" % tuple(parts[:5]))
    out.write("\n\n")


def write_footer(out, numruns, comment):
    out.write("registerBenchmark(\"%s\", algLinSearch, structureIPv4, rs, headers, %d)\n" % (comment, numruns))
    out.write("registerBenchmark(\"%s\", algBitvector, structureIPv4, rs, headers, %d)\n" % (comment, numruns))
    out.write("registerBenchmark(\"%s\", algTuples, structureIPv4, rs, headers, %d)\n\n" % (comment, numruns))
    out.write("registerBenchmark(\"%s\", algHiCuts, structureIPv4, rs, headers, %d)\n" % (comment, numruns))


def main(rulefile, tracefile, outfile, numruns, commena, binth):
    with open(outfile, "w") as out:
        write_header(out, binth)
        write_rules(out, rulefile)
        write_trace(out, tracefile)
        write_footer(out, numruns, comment)


if __name__ == "__main__":
    if len(sys.argv) < 4:
        print "Usage: %s <rulesfile> <tracefile> <outfile> [numruns] [comment] [binth]" % sys.argv[0]
        exit(1)

    rulefile = sys.argv[1]
    tracefile = sys.argv[2]
    outfile = sys.argv[3]
    numruns = 8
    comment = ""
    binth=16
    if len(sys.argv) >= 5:
        numruns = int(sys.argv[4])
    if len(sys.argv) >= 6:
        comment = sys.argv[5]
    if len(sys.argv) >= 7:
        binth = sys.argv[6]
        
    main(rulefile, tracefile, outfile, numruns, comment, binth)

