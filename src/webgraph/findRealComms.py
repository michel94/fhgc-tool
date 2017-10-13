import sys

dataset = sys.argv[1]

def loadClustersFor(t):
	global dataset
	clustering = {}
	with open('../faststep/' + dataset + '-tests/communities/communities-' + str(t) + 'div.txt', 'r') as f:
		i = 0
		while True:
			s = f.readline()
			if s.strip() == '':
				break
			c = int(s)
			if c not in clustering:
				clustering[c] = set()
			clustering[c].add(i)
			i+=1
	return clustering

d = {}

clusters = []

print(dataset)
indexes = list(map(int, open('data/' + dataset + '/random-perm/indexes.txt').read().split()))
c = 0
sizes = {}
while True:
	try:
		r = set(map(int, input().split()))
		r = {indexes[i-1] for i in r}
		clusters.append(r)
	except Exception as e:
		break
	for i in r:
		d[i] = c

	c+=1

clusters = sorted(clusters, key=len)

thresholds = [10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000]
clusterings = {}
for t in thresholds:
	clusterings[t] = loadClustersFor(t)

jaccards = []
#common = []

for c in clusters:
	size = len(c)
	if size < 100:
		continue
	i = 0
	best = 0
	while i < len(thresholds):
		t = thresholds[i]
		#print('Cluster of size %d, looking for a match in clustering with cutoff of size %d' % (size, t))
		
		for elements in clusterings[t].values():
			tmp = len(elements & c) / len(elements | c)
			if tmp > best:
				best = tmp

		i += 1
	jaccards.append((best, size))
	#print(best / len(c))

jaccards = [(i/j, j) for i,j in jaccards]
print(sorted(jaccards))
