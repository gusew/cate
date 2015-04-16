import sys
import re

def parse_ip(out, addr, mask):
    if int(mask) == 32:
        out.write("ruleAtomExact(ipv4Toi(\"%s\"))" % addr)
    else:
        out.write("ruleAtomPrefix(ipv4Toi(\"%s\"), maskToi(%s))" % (addr, mask))


def split_subnet(subnet):
    return subnet.split("/")


def write_header(out, tuples):
    out.write("algLinSearch = createAlgorithm(\"LinearSearch%dtpl.so\", {10000})\n" % tuples)
    out.write("algBitvector = createAlgorithm(\"Bitvector%dtpl.so\", {10000})\n" % tuples)
    out.write("algTuples = createAlgorithm(\"TupleSpace%dtpl.so\", {10000, 97})\n" % tuples)
    out.write("algHiCuts = createAlgorithm(\"HiCuts%dtpl.so\", {10000, %d, 3.0})\n\n" % (tuples, 10*int(tuples)))

    out.write("structure = {")
    t = 0
    while t < tuples:
        out.write("32")
        if t < (tuples-1):
            out.write(", ")
        t += 1
    out.write("}\n\n")


def write_rules(out, rulefile):
    with open(rulefile, "r") as f:
        lines = f.readlines()
    
    out.write("rs = createRuleset()\n")
    for line in lines:
        line = line.strip()
        parts = re.split("[ \t]+", line)
        
        out.write("addRuleToRuleset(rs, {")

        idx = 0
        while idx < len(parts):
            addr, mask = split_subnet(parts[idx])
            parse_ip(out, addr, mask)
            if idx < (len(parts)-1):
                out.write(", ")
            idx += 1

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

        idx = 1
        while idx < len(parts):
            out.write("%s" % parts[idx])
            if idx < (len(parts)-1):
                out.write(", ")
            idx += 1

        out.write("})\n")
    out.write("\n\n")


def write_footer(out, numruns):
    out.write("registerBenchmark(\"Linear Search\", algLinSearch, structure, rs, headers, %d)\n" % numruns)
    out.write("registerBenchmark(\"Bitvector\", algBitvector, structure, rs, headers, %d)\n" % numruns)
    out.write("registerBenchmark(\"Tuple Space Search\", algTuples, structure, rs, headers, %d)\n" % numruns)
    out.write("registerBenchmark(\"HiCuts\", algHiCuts, structure, rs, headers, %d)\n\n" % numruns)


def main(rulefile, tracefile, outfile, numruns, tuples):
    with open(outfile, "w") as out:
        write_header(out, tuples)
        write_rules(out, rulefile)
        write_trace(out, tracefile)
        write_footer(out, numruns)


if __name__ == "__main__":
    if len(sys.argv) < 6:
        print "Usage: %s <rulesfile> <tracefile> <outfile> <numruns> <tuples>" % sys.argv[0]
        exit(1)

    rulefile = sys.argv[1]
    tracefile = sys.argv[2]
    outfile = sys.argv[3]
    numruns = int(sys.argv[4])
    tuples = int(sys.argv[5])
        
    main(rulefile, tracefile, outfile, numruns, tuples)

