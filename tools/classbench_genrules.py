import sys
import random

PROTOCOL = 0x6

def main(n):
    rules = []
    for _ in xrange(n):
        byte1 = random.randrange(256)
        byte2 = random.randrange(256)
        src_net = "141.20.%d.%d/16" % (byte1, byte2)

        byte1 = random.randrange(256)
        byte2 = random.randrange(256)
        dst_net = "1.2.%d.%d/16" % (byte1, byte2)

        src_port = random.randrange(65536)
        dst_port = random.randrange(65536)

        prot = PROTOCOL

        cb_rule = "@%s %s %d : %d %d : %d %s/0xFF" % (src_net, dst_net,
                src_port, src_port, dst_port, dst_port, hex(prot))
        rules.append(cb_rule)
    rule_src = "\n".join(rules)
    print rule_src


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Usage: %s <rules-amount> [<random seed>]" % sys.argv[0]
        exit(1)
    n = int(sys.argv[1])
    seed = 1
    if len(sys.argv) > 2:
        random.seed(int(sys.argv[2]))
    random.seed(seed)
    main(n)
