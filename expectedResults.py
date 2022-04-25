import sys

result = set()


def readDataFile(readFile, writeFile):
	f = open(readFile,"r");
	wf = open(writeFile,"a");
	
	for line in f:
		a = line.split() 
		if(len(a) < 1):
			continue;
		if(a[0] == "I"):
			result.add(int(a[1]));
			wf.write("Insert " + a[1] + "\n");
		elif(a[0] == "D"):
			if(len(a) == 2) :
				result.discard(int(a[1]));
				wf.write("Delete " + a[1] + "\n")
			elif(len(a) == 3) :
				low = int(a[1])
				high = int(a[2]) + 1
				dRange = list(range(low,high)).sort()

				wf.write("Deleted: [" );
				for i in dRange :
					if( i in result):
						result.discard(i)
						wf.write( str(i) + " ");
				wf.write("]\n"); 
		elif(a[0] == "Q"):
			#query
			if(int(a[1]) in result) :
				wf.write("Found " + a[1] + "\n")
			else:
				wf.write("Did not find " + a[1] + "\n")
		elif(a[0] == "S"):
			#range scan
			low = a[1]
			high = a[2]
			selectRange = range(int(low),int(high))
			rangeScanResult = result.intersection(selectRange)
			if(len(rangeScanResult) > 0 ):
				wf.write("Found rangeScan [" );
				for i in rangeScanResult :
					wf.write( str(i) + " ");

				wf.write("]\n");
			else:
				 wf.write("Did not find rangeScan [" + low + ", " + high + "]\n");


def main(args):
	print("Inside")
	if ('-h' in args) or not(len(args) == 3):
		print('USAGE:\n\t%s <inputFile> <outputFile>' % (args[0]))
		sys.exit(0)
	readDataFile(args[1],args[2]);

if __name__ == '__main__':
    main(sys.argv)
	






