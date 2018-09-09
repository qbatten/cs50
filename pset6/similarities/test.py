from helpers import distances

matrix = distances("aaa", "bbb")

for i in range(len("aaa") + 2):
	for j in range(len("bbb") + 2):
		print(matrix[i][j], end = " | ")
	print("|")