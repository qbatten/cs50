from cs50 import get_int

while True:
    n = get_int("Height: ")
    if n >= 0 and n <= 23:
        break
# rows
i = 1
for i in range(n):
    for j in range(n - i - 1):
        print(" ", end="")
    for j in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for j in range(i + 1):
        print("#", end="")
    print()