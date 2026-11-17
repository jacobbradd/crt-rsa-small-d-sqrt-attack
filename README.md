# Fast Polynomial Multiplication and CRT-RSA

This is a writeup of [this math overflow post](https://mathoverflow.net/questions/120160/attack-on-crt-rsa) detailing a basic attack on CRT-RSA that is $O(\min(\sqrt{d_p}, \sqrt{d_q}))$. Note that this is not the same as Coppersmith attacks. I was working on a PicoCTF challenge at the time and became interested in this problem. I will detail it better when I have a chance to recall (as well as reorganise this repo). It is based on methods described in the book "Modern Computer Algebra" by von zur Gathen and Gerhard.

Said plainly: for a given small enough bit length $b$, it can break CRT-RSA with a public key $(N,e)$ such that $d_p$ or $d_q$ has bit length at most $2b$. I would estimate this approach might handle $b = 20$ (so $d_p$ bit length at most $40$). This is not nearly as strong as Coppersmith. For comparison, the current best practical Coppersmith attack (as of 2025) for 1024 bit moduli is $d_p, d_q < N^{0.067}$ which is a bit length of around $69$.

Mathematically, the idea is simple: the hope is that, for a random integer $x$ and some fixed $D$ with $D^2 > d_p$, it becomes likely that $\gcd(x^{ed_p} - x, N)$ is a prime factor of $N$ (under "good" circumstances), because $x^{ed_p} - x$ is a multiple of $p$. Writing $d_p = a + bD$ (since $d_p < D^2$), this means $\gcd(x^{ea}x^{ebD} - x, N)$ will be a prime factor. We don't know $a$ and $b$, but we just guess all of them: compute $\gcd(\prod_{i=1}^D(x^{ei}x^{ebD} - x), N)$ for each value of $b$. This can be done quickly with multipoint evaluation and subproduct trees for fast multiproducts.


There is a particularly interesting operation called fast multipoint evaluation, but it's currently implemented in `main.cpp` and should be separated out. The idea is that evaluation $p(a)$ may be described as $p(x)$ mod $x-a$, so multievaluation is done by considering modulo by "subproducts", that is, products of these $x-a_i$. You then build a "subproduct tree" from the ground up (starting with $x-a_i$ at the leaves), and then compute $p(x)$ modulo each term in the subproduct tree by going down the tree.

I wrote big integer, modular arithmetic and polynomial arithmetic classes in C++, and experimented with a Karatsuba (divide and conquer) multiplication algorithm in Python. 

At some point, when I started using Linux more, it occured to me that I should use a library. I used the GNU Multiprecision library (GMP), in particular its C++ wrapper gmpxx. 

At the time I didn't know about FLINT, so I still implemented polynomials and modular arithmetic manually. A FLINT implementation is given where you can input $N$ and $e$ yourself. It is, of course, much faster.



To install GMP on Ubuntu use `sudo apt install -y libgmp-dev`
To install FLINT see [the instructions](https://github.com/flintlib/flint#building-from-source)
