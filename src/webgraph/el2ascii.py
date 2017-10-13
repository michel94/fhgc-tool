n, _ = input().split()
n = int(n)
l = [[] for i in range(n)]
while True:
	try:
		a, b = input().split()
		a = int(a)
		b = int(b)
		l[a].append(b)
	except Exception as e:
		break

for r in range(len(l)):
	for i in l[r]:
		print(r+1, i+1)
