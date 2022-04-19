result = set()

def readDataFile(readFile, writeFile):
	f = open(readFile,"r");
	wf = open(writeFile,"a");
	
	for line in f:
		print(line)
		a = line.split() 
		if(len(a) == 1):
			continue;
		if(a[0] == "I"):
			result.add(a[1]);
			wf.write("Insert " + a[1] + " || currSize = " + str(len(result)) + "\n");
		elif(a[0] == "D"):
			if(len(a) == 3) :
				result.discard(a[1]);
				wf.write("Delete " + a[1] + " || currSize = " +str(len(result))+ "\n")
			elif(len(a) == 4) :
				low = a[2]
				high = a[3]
				dRange = range(low,high)
				for d in dRange:
					result.discard(d)
					wf.write("Delete " + d + " || currSize = " +str(len(result))+ "\n")
		elif(a[0] == "Q"):
			#query
			result.find(a[1]);
			wf.write("Found  " + a[1] + "\n")
		elif(a[0] == "S"):
			#range scan
			low = a[1]
			high = a[2]
			selectRange = range(low,high)
			rangeScanResult = result.intersection(selectRange)
			wf.write("Found rangeScan " + rangeScanResult + "\n")



def main():
	print("Inside")
	readDataFile("data.wl","expectedResults.txt");
	readDataFile("rangeQuerySearch.wl","expectedResults.txt")

main();
	







