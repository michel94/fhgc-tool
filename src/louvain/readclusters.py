import sys
from sys import stderr

parents = []
children = []
size = 0


def getParent(i, n):
	global parents
	
	for level in range(n):
		i = parents[level][i]
		level += 1
	return i

def clusters(outPath, level, nodes):
	'''
	if level == 0:
		curCluster = [i for i in range(len(nodes))]
	
	s = set()
	for i in d:
		s.add(( len(d[i]), i))
	for i in range(10):
		m = max(s)
		print(m)
		s.remove(m)
	'''
	global parents, size, children
	if level == 0:
		size = len(nodes)
	parents.append([0] * size)
	for i, c in nodes:
		parents[level][i] = c

	nClusters = len(set([c for i, c in nodes]))
	children.append([[] for i in range(nClusters)])
	for i, c in nodes:
		children[level][c].append(i)

	if level > 0:
		with open(outPath + "-" + str(level-1) + ".txt", 'w') as f:
			for i in range(size):
				f.write(str(getParent(i, n)) + "\n")

LABEL = 0
labels = []

def dfs(n, i):
	global LABEL, children
	#print(n, i)
	if n < 0:
		labels[i] = LABEL
		LABEL += 1
		return

	for j in children[n][i]:
		dfs(n-1, j)

last = 0
n = 0
l = []

with open(sys.argv[1], 'r') as f:
	for line in f:
		a, b = line.split()
		a = int(a)
		b = int(b)
		if a < last:
			#print("level", n, "size", len(l), file=stderr)
			clusters(sys.argv[2], n, l)
			n += 1
			l = []
		l.append((a, b))
		last = a

#print("level", n, "size", len(l), file=stderr)
clusters(sys.argv[2], n, l)
n += 1

labels = [-1] * size

for i in range(len(children[-1]) ):
	dfs(n-1, i)

for i in range(size):
	print(str(labels[i]))

count = {}
for i in labels:
	if i not in count:
		count[i] = 1
	else:
		#print('number', i, 'already found', count[i], 'times', file=stderr)
		exit(1)
		count[i] += 1
