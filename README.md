# Fast Polynomial Multiplication and CRT-RSA

This is a writeup of [this math overflow post](https://mathoverflow.net/questions/120160/attack-on-crt-rsa) detailing a basic attack on CRT-RSA that is $O(\sqrt{d})$. Note that this is not the same as Coppersmith attacks. I was working on a PicoCTF challenge at the time and became interested in this problem. I will detail it better when I have a chance to recall (as well as reorganise this repo). It is based on methods described in the book "Modern Computer Algebra" by von zur Gathen and Gerhard.

There is a particularly interesting operation called fast multipoint evaluation, but it's currently implemented in `main.cpp` and should be separated out. The idea is that evaluation $p(a)$ may be described as $p(x)$ mod $x-a$, so multievaluation is done by considering modulo by "subproducts", that is, products of these $x-a_i$. You then build a "subproduct tree" from the ground up (starting with $x-a_i$ at the leaves), and then compute $p(x)$ modulo each term in the subproduct tree by going down the tree.

I wrote big integer, modular arithmetic and polynomial arithmetic classes in C++, and experimented with a Karatsuba (divide and conquer) multiplication algorithm in Python. 

At some point, when I started using Linux more, it occured to me that I should use a library. I used the GNU Multiprecision library (GMP), in particular its C++ wrapper gmpxx. 

At the time I didn't know about FLINT, so I still implemented polynomials and modular arithmetic manually. A FLINT implementation is given where you can input $N$ and $e$ yourself. It is, of course, much faster.



To install GMP on Ubuntu use `sudo apt install -y libgmp-dev`
To install FLINT see [the instructions](https://github.com/flintlib/flint#building-from-source)
