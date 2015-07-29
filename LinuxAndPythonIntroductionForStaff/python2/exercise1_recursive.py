# answer to the exercice 1 wiht a different method
# this is called a recursive method (i.e. the function calls itself)


def fibonnaci(n):
    if (n==0):
        return 0
    elif (n==1):
        return 1
    else:
        return fibonnaci(n-1) + fibonnaci(n-2)
    
print fibonnaci(7)
