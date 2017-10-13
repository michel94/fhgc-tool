import sys
d = {}

dataset = sys.argv[1]
indexes = list(map(int, open('data/' + dataset + '/random-perm/indexes.txt').read().split()))
c = 0
while True:
	try:
		r = list(map(int, input().split()))
		r = [indexes[i-1] for i in r]
	except Exception as e:
		break
	for i in r:
		d[i] = c

	c+=1

m = max(d.keys())
for i in range(m+1):
	if i in d:
		print(d[i])
	else:
		print(c)

