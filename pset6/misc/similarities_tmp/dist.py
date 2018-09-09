import sys
from enum import Enum

class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())

#init vars
lena = int(sys.argv[1])
lenb = int(sys.argv[2])
print(lena, lenb)
cellInit = (0, Operation.DELETED)

# fill matrix with starter tuples
m = [[cellInit] * (lena + 1) for i in range(lenb + 1)]

# top left corner (aka "" to "")
m[0][0] = (0, )

# fill last column
for i in range(1, lenb + 1):
    m[i][0] = ((Operation.DELETED.value + m[i - 1][0][0]), Operation.DELETED.name)

# fill last row
for j in range(1, lena + 1):
    m[0][j] = ((Operation.INSERTED.value + m[0][j - 1][0]), Operation.INSERTED.name)


# ALGORITHMMMM
for i in range(1, lena + 1):
    for j in range(1, lenb + 1):
        # find min option
        left = ((m[i][j - 1][0] + Operation.INSERTED.value), Operation.INSERTED.name)
        ans = left

        diag = ((m[i - 1][j - 1][0] + Operation.SUBSTITUTED.value), Operation.SUBSTITUTED.name)
        if diag[0] < left[0]:
            ans = diag
        # import pdb; pdb.set_trace()
        above = ((m[i - 1][j][0] + Operation.DELETED.value), Operation.DELETED.name)
        if above[0] < left[0]:
            ans = above

        m[i][j] = ans
        del left, diag, above

# print the whole table
for i in range(lenb + 1):
    for j in range (lena + 1):
        print(f"{m[i][j]} |", end = "")
    print("|")
