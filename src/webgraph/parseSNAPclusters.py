from sys import argv

if len(argv) < 3:
	print('Usage: python parseSNAPclusters.py <file-input-1> <file-output-1> [--sub0]')
	exit(1)

fIn = ""
fOut = ""
sub = 0
for a in argv:
	if a == '--sub0':
		sub = 0
	elif fIn == '':
		fIn = a
	else:
		fOut = a

def getIdx(s):
	global sub
	return int(s)-sub

clusters = []
m = 0
with open(argv[1], 'r') as f:
	while True:
		idx = f.readline().split()
		l = []
		for i in idx:
			try:
				l.append(getIdx(i))
			except Exception:
				pass
		
		if len(l) == 0:
			break
		m = max(max(*l), m)
		if len(l) > 2:
			clusters.append(l)

print('Clusters', len(clusters))

indexes = [[] for i in range(m+1)]
for ic, c in enumerate(clusters):
	for i in c:
		indexes[i].append(ic)

data = []
for idx in indexes:
	#if len(idx) > 0:
	data.append(' '.join([str(i) for i in idx]))

with open(argv[2], 'w') as f:
	f.write('\n'.join(data[1:]))
	f.write('\n')
