import sys

def merge_headers(out, lfile, rfile):
    with open(lfile, "r") as l:
        linesl = l.readlines()
        
    with open(rfile, "r") as r:
        linesr = r.readlines()

    indexl = 0
    indexr = 0

    while indexl < len(linesl) and indexr < len(linesr):
        left = linesl[indexl].strip()
        right = linesr[indexr].strip()

        partsl = left.split(" ")
        partsr = right.split(" ")

        ruleidxl = int(partsl[0])
        ruleidxr = int(partsr[0])

        if ruleidxl == ruleidxr:
            out.write("%d " % ruleidxl)

            partidx = 1
            while partidx < len(partsl):
                out.write("%s " % partsl[partidx])
                partidx += 1
            
            partidx = 1
            while partidx < len(partsr):
                out.write("%s " % partsr[partidx])
                partidx += 1
                
            out.write("\n")

            indexl += 1
            indexr += 1
        elif ruleidxl > ruleidxr:
            indexr += 1
        else:
            indexl += 1
        

def main(lfile, rfile, outfile):
    with open(outfile, "w") as out:
        merge_headers(out, lfile, rfile)

if __name__ == "__main__":
    if len(sys.argv) < 4:
        print "Usage: %s <file1> <file2> <outfile>" % sys.argv[0]
        exit(1)

    leftfile = sys.argv[1]
    rightfile = sys.argv[2]
    outfile = sys.argv[3]
        
    main(leftfile, rightfile, outfile)

