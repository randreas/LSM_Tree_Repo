result = set()

def readDataFile(readFile, writeFile):
	f = open(readFile,"r");
	wf = open(writeFile,"a");
	
	for line in f:
		a = line.split() 
		if(len(a) == 1):
			continue;
		if(a[0] == "I"):
			result.add(int(a[1]));
			wf.write("Insert " + a[1] + " || currSize = " + str(len(result)) + "\n");
		elif(a[0] == "D"):
			if(len(a) == 3) :
				result.discard(int(a[1]));
				wf.write("Delete " + a[1] + " || currSize = " +str(len(result))+ "\n")
			elif(len(a) == 4) :
				low = int(a[2])
				high = int(a[3])
				dRange = range(low,high)
				for d in dRange:
					result.discard(d)
					wf.write("Delete " + d + " || currSize = " +str(len(result))+ "\n")
		elif(a[0] == "Q"):
			#query
			result.find(int(a[1]));
			wf.write("Found  " + a[1] + "\n")
		elif(a[0] == "S"):
			#range scan
			low = a[1]
			high = a[2]
			selectRange = range(int(low),int(high))
			rangeScanResult = result.intersection(selectRange)
			print(rangeScanResult)
			wf.write("Found rangeScan [" 
			for i in rangeScanResult :
				wf.write( str(i) + " ");

			wf.write("]\n"); 


def main():
	print("Inside")
	readDataFile("data.wl","expectedResults.txt");
	readDataFile("rangeQuerySearch.wl","expectedResults.txt")

main();
	







