import sys


def merge_rules(out, lfile, rfile):
    with open(lfile, "r") as l:
        linesl = l.readlines()
        
    with open(rfile, "r") as r:
        linesr = r.readlines()

    indexl = 0
    indexr = 0

    while indexl < len(linesl) and indexr < len(linesr):
        left = linesl[indexl].strip()
        right = linesr[indexr].strip()

        out.write("%s %s\n" % (left, right))

        indexl += 1
        indexr += 1

def main(lfile, rfile, outfile):
    with open(outfile, "w") as out:
        merge_rules(out, lfile, rfile)

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print "Usage: %s <file1> <file2> <outfile>" % sys.argv[0]
        exit(1)

    leftfile = sys.argv[1]
    rightfile = sys.argv[2]
    outfile = sys.argv[3]
        
    main(leftfile, rightfile, outfile)

