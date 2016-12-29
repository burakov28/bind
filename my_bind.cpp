#include <iostream>
#include <functional>

template < int N >
struct place_holder {};

place_holder<0> _1;
place_holder<1> _2;
place_holder<2> _3;
place_holder<3> _4;
place_holder<4> _5;



template <typename F, typename... Args>
struct bind_function {
    
    typedef std::tuple<Args...> my_tuple;
    
    
    F&& func;
    my_tuple initial_args;

    template <typename ctorF, typename... ctorArgs>
    bind_function(ctorF&& _func, ctorArgs&&... ctor_args) : func(std::forward<ctorF>(_func)), initial_args(std::forward<ctorArgs>(ctor_args)...) {}

    template <typename initial_arg, typename... newArgs>
    decltype(auto) calc(initial_arg& carg, newArgs&&...) {
        return carg;
    }

    template <typename bindF, typename... bindArgs, typename... newArgs>
    decltype(auto) calc(bind_function<bindF, bindArgs...>& inner_bind, newArgs&&... new_args) {
        return inner_bind(std::forward<newArgs>(new_args)...);
    }

    template <int N, typename... newArgs>
    decltype(auto) calc(place_holder <N>&, newArgs&&... new_args) {
        return std::get<N>(std::forward_as_tuple(std::forward<newArgs>(new_args)...));
    }

    template <int... S>
    struct sequence {};

    template <int S, int... Rest>
    struct seq_gen {
        typedef typename seq_gen <S - 1, S - 1, Rest...>::type type;
    };

    template <int... Rest>
    struct seq_gen <0, Rest...> {
        typedef sequence <Rest...> type;
    };

    template <typename... newArgs, int... S>
    auto call_with_seq(const sequence <S...> &, newArgs&&... new_args) {
        return func(calc(std::get<S>(initial_args), new_args...)...);
    }

    template <typename... newArgs>
    auto operator() (newArgs&&... new_args) {
        return call_with_seq(typename seq_gen<std::tuple_size<my_tuple>::value>::type(), std::forward<newArgs>(new_args)...);
    }
};


template <typename F, typename... Args>
bind_function<F, Args...> bind(F&& func, Args&&... args) {
    return bind_function<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
}

int mult(double a, int b, double c) {
    return (int) (a * b * c);
}

using std::cout;
using std::endl;


void print() {
    cout << "Hello" << endl;      
}   

int main() {
        
    cout << bind(&mult, _1, _2, _1)(0.5, 100) << endl;;
    bind(&print)();
    return 0;
}