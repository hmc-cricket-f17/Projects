#include "keygen.hpp"

#include <boost/random.hpp>

#include <stack>
#include <memory>
#include <chrono>
#include <cassert>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

using generator512 = boost::random::independent_bits_engine<mt19937, 512, cpp_int>;

#define FAIL cpp_int(-1)

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

/* 
 * Checks if (D, phi(N)) == 1 and if so, finds E s.t. DE - phi(N)F = 1 
 * Returns cpp_int(-1) if (D, phi(N)) != 1 
 * Undefined behavior if d <= 1 or phi <= 1
 */
cpp_int findE(cpp_int d, cpp_int phi)
{
    // Going to lose phi's value while doing Euclidean Alg, and might need it later.
    cpp_int phi_copy{phi};
    cpp_int d_copy{d}; // TESTING ONLY, REMOVE WHEN DONE

    // Let a be the greater of d and phi, b be the lesser
    bool d_greater;
    cpp_int* a;
    cpp_int* b;
    if (d > phi) {
        a = &d;
        b = &phi;
        d_greater = true;
    } else {
        a = &phi;
        b = &d;
        d_greater = false;
    }
    cpp_int mod;
    cpp_int* modptr = &mod;

    /* Euclidean alg: (a, b) = (b, a mod b). Store quotients in stack */
    stack<cpp_int> quotients;
    while (*b != 0){
        quotients.emplace(*a / *b);
        *modptr = *a % *b;

        // Reuse these 3 
        cpp_int* tmp = a;
        a = b;
        b = modptr;
        modptr = tmp;
    }

    // a now equals gcd(d, phi(N)). If a != 1, we need to try again.
    if (*a != 1){
        return FAIL;
    }

    /* Reverse Euclidean alg to find x,y s.t. ax + by = gcd(a,b) = 1 */

    // Pop last quotient, we don't need it
    quotients.pop(); 

    // To figure out which # goes with a and which goes with b (see below)
    bool even_size = quotients.size() % 2 == 0; 

    // Accumulate x and y by adding -quotient on top of stack times x or y
    cpp_int x{1};
    cpp_int y{-quotients.top()};

    quotients.pop();

    while(!quotients.empty()){
        x += -y * quotients.top();
        quotients.pop();
        if (!quotients.empty()){
            y += -x * quotients.top();
            quotients.pop();
        }
    }

    cout << "x=" << x << endl;
    cout << "y=" << y << endl;

    // We start the reverse alg with 1 = 1(a) - 1(b), so x is positive and y is negative.
    // If even, we finish with ay + bx = 1
    // If odd, we finish with ax + by = 1
    // If d > phi, then a = d & b = phi
    // If d < phi, then a = phi & b = d

    // Therefore:
    // 1) even & dGreater -> dy + phi(x)
    // 2) even & phiGreater -> phi(y) + dx
    // 3) odd & dGreater -> dx + phi(y)
    // 4) odd & phiGreater -> phi(x) + dy
    // Since 1) == 4) & 2) == 3), we can just xor even and dGreater
    // to find out if e is x or y.
    cout << "dx + phi(y) == " << d_copy*x + phi_copy*y << endl;
    cout << "dy + phi(x) == " << d_copy*y + phi_copy*x << endl;

    bool e_is_x = even_size ^ d_greater;
    if (e_is_x) {

        cpp_int e{x};
        cpp_int f{y};
        cout << "d=" << d_copy << endl;
        cout << "e=" << e << endl;
        cout << "f=" << f << endl;

        cpp_int gcd = (d_copy * e) + (f * phi_copy);
        assert(gcd == 1);

        return x;
    }

    cpp_int e{y + phi_copy};
    cpp_int f{x - d_copy};
    cout << "d=" << d_copy << endl;
    cout << "e=" << e << endl;
    cout << "f=" << f << endl;

    cpp_int gcd = (d_copy * e) + (f * phi_copy);
    assert(gcd == 1);

    return y + phi_copy;
}

// Private key d first, public key e second
array<cpp_int, 2> generate_keys(const cpp_int& phi)
{
    // generator512 gen{seed};
    srand (time(NULL));

    cpp_int d{rand() % 100};
    cpp_int e{findE(d, phi)};

    while (e == FAIL) {
        d = rand() % 100;
        e = findE(d, phi);
    }

    return array<cpp_int, 2> {{move(d), move(e)}};
}

// int main(int argc, char** argv)
// {
//     auto keys = generate_keys(atoi(argv[1]));

//     cout << "d=" << keys[0] << endl;
//     cout << "e=" << keys[1] << endl;
// }
