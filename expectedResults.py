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
				dRange = range(low,high)
				wf.write("Deleted size = "  + str(len(dRange)) + "\n");
				#print(dRange);
				# wf.write("Deleted size: [" );
				# for i in dRange :
				# 	result.discard(i)
				# 	wf.write( str(i) + " ");
				# wf.write("]\n"); 
		elif(a[0] == "Q"):
			#query
			if(int(a[1]) in result) :
				wf.write("Found " + a[1] + "\n")
			else:
				wf.write("Did not Find " + a[1] + "\n")
		elif(a[0] == "S"):
			#range scan
			low = a[1]
			high = a[2]
			selectRange = range(int(low),int(high))
			#print(selectRange);
			rangeScanResult = result.intersection(selectRange)
			wf.write("Found size = "  + str(len(rangeScanResult)) + "\n");
			# wf.write("Found rangeScan [" );
			# for i in rangeScanResult :
			# 	wf.write( str(i) + " ");

			# wf.write("]\n"); 


def main(args):
	print("Inside")
	if ('-h' in args) or not(len(args) == 3):
		print('USAGE:\n\t%s <inputFile> <outputFile>' % (args[0]))
		sys.exit(0)
	readDataFile(args[1],args[2]);

if __name__ == '__main__':
    main(sys.argv)
	






