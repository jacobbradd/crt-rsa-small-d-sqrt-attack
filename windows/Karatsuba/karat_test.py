#Multiply two polynomials as arrays of 2^N numbers

def karatsuba(p,q):
    i = 1
    while i < len(p) or i < len(q):
        i*=2
    if i > len(p):
        p += [0 for j in range(i-len(p))]
    if i > len(q):
        q += [0 for j in range(i-len(q))]
    return karatsuba_algo(p,q)

def karatsuba_algo(p,q):
    N = len(p)
    if N == 1:
        return [0,p[0]*q[0]]
    m1 = karatsuba(p[0:N//2],q[0:N//2])
    m2 = karatsuba(p[N//2:],q[N//2:])
    m = m1 + m2
    s1 = p[0:N//2]
    s2 = q[0:N//2]
    for i in range(N//2):
        s1[i] += q[N//2+i]
        s2[i] += p[N//2+i]
    s = karatsuba(s1,s2)
    for i in range(N):
        try:
            m[N//2+i] += s[i] - m1[i] - m2[i]
        except:
            print("fail: ",m,s,m1,m2,N,i)
            exit()
    return m  
        
    