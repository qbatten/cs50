from enum import Enum

COSTID = 1

class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""
    # init vars
    lenb = len(b)
    lena = len(a)
    cellInit = (0, "None")

    # fill matrix with starter tuples
    m = [[cellInit] * (lenb + 1) for i in range(lena + 1)]

    # top left corner (aka "" to "")
    m[0][0] = (0, 0)

    # fill first column
    for i in range(1, lena + 1):
        m[i][0] = ((COSTID + m[i - 1][0][0]), Operation.DELETED)
    # fill top row
    for j in range(1, lenb + 1):
        m[0][j] = ((COSTID + m[0][j - 1][0]), Operation.INSERTED)

    # ALGORITHM
    for i in range(1, lena + 1):
        for j in range(1, lenb + 1):

            # diag AKA substitute
            if a[i - 1] == b[j - 1]:
                diag = (m[i - 1][j - 1][0], "None")
            else:
                diag = (m[i - 1][j - 1][0] + COSTID, Operation.SUBSTITUTED)
            ans = diag

            # left AKA insert
            left = (m[i][j - 1][0] + COSTID, Operation.INSERTED)
            if left[0] < ans[0]:
                ans = left
            # above AKA delete
            above = (m[i - 1][j][0] + COSTID, Operation.DELETED)
            if above[0] < ans[0]:
                ans = above

            m[i][j] = ans
            #del diag, left, above

    return m
