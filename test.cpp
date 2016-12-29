#include <iostream>

template <typename F>
F gt(F a) {
    typedef F& type;
    return a;
}


using namespace std;

int main() {
    typename gt< int >::type a = 5;
    cout << a << endl;
    return 0;
}