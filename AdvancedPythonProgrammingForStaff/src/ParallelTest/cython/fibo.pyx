cpdef fibo(n):
    if n == 0 or n == 1:
        return n
    return fibo(n - 2) + fibo(n - 1)

