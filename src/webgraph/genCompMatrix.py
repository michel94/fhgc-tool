from __future__ import print_function
import sys

FS = 'faststep'
LLP = 'llp'
LOUVAIN = 'louvain'

a1, a2 = (None, None)
results = None


sortFunc = {
	FS: lambda s: -s,
	LOUVAIN: lambda s: -s,
	LLP: lambda s: s,
}

def printMatrix(results, a1, a2, plot=False):
	header = False
	COLSIZE = 10
	print(a1, a2)
	al1keys = sorted(results.keys(), key=sortFunc[a1])
	for i in al1keys:
		al2keys = sorted(results[i].keys(), key=sortFunc[a2])
		if not header:
			for k in [''] + al2keys:
				size = len(str(k))
				print(k, end=' ' * (COLSIZE - size) )
			print()
			header = True
		print(i, end=':\t')
		for j in al2keys:
			print("%.7lf" % results[i][j], end=' ')
		print()

	if plot:
		res = []
		al1keys = sorted(results.keys(), key=sortFunc[a1])
		for i in al1keys:
			al2keys = sorted(results[i].keys(), key=sortFunc[a2])
			res.append([results[i][j] for j in al2keys])
		
		#drawMatrix(res)
		#print(a1, a2, [])
		f = open('mat.txt', 'w')
		f.write("%s %s\n" % (a1, a2))
		f.write("%s\n" % str(res))
		f.close()
		#return (res, al1keys, al2keys, a1, a2)

	return None

def handleData(argv):
	plot = False

	global results, a1, a2
	with open(argv[1], 'r') as f:
		if len(argv) > 2 and argv[2] == '--image':
			plot = True
		while True:
			r = f.readline()
			if r.strip() == '':
				return printMatrix(results, a1, a2, plot)
				break
			elif r.startswith('communities-'):
				r = r.split()
				a = r[0][12:-4]
				if a1 == FS:
					a = a[:-3]
				b = r[1][12:-5]
				if a2 == FS:
					b = b[:-3]
				a = int(a)
				b = int(b)
				if a not in results:
					results[a] = {}
				results[a][b] = float(r[2])
				#print(results[-1])
			else:
				if results != None:
					printMatrix(results, a1, a2, plot)
				a1, a2 = r.split(' with ')
				a1 = a1.lower()
				a2 = a2.lower()
				print(r, end='')
				a1 = a1.strip()
				a2 = a2.strip()
				results = {}



#print(results)

if __name__ == '__main__':
	if len(sys.argv) < 2:
		print('Usage: python genCompMatrix.py <dataset>-nmi.txt [--image]')
		exit(1)
	handleData(sys.argv)
