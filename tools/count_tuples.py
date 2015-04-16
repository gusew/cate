import sys

def main(lines):
    tuples = set()
    for line in lines:
        parts = line.split(" ")
        t = (int(parts[0].split("/")[1]), int(parts[1].split("/")[1]))
        tuples.add(t)
    for t in tuples:
        print t
    print "The rule set has %d tuples!" % (len(tuples))


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        lines = f.readlines()
    main(lines)
