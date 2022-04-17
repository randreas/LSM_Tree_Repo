result = set()

def readDataFile(readFile, writeFile):
	f = open(readFile,"r");
	wf = open(writeFile,"x");
	lines = f.readLines()

	for line in lines:
		a = line.split() 
		if(a[0] == "I"):
			result.add(a[1]);
			wf.write("Insert " + a[1] + "currSize = " +result.size());
		elif(a[1] == "D"):
			if(a.size == 3) :
				result.discard(a[1]);
				wf.write("Delete " + a[1] + "currSize = " +result.size());
			elif(a.size == 4) :
				low = a[2]
				high = a[3]
				dRange = range(low,high)
				for d in dRange:
					result.discard(d)
					wf.write("Delete " + d + "currSize = " +result.size());
		elif(a[1] == "Q"):
			#query
			result.find(a[1]);
			wf.write("Found  " + a[1]);
		elif(a[1] == "S"):
			#range scan
			low = a[2]
			high = a[3]
			selectRange = range(low,high)
			rangeScanResult = result.intersection(selectRange)
			wf.write("Found rangeScan " + rangeScanResult);



def main():
	readDataFile("data.wl","expectedResults.txt");
	readDataFile("test_10000_3.wl","expectedResults.txt")







