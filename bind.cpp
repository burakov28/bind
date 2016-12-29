

#include <functional>
#include <iostream>
#include <string>


template < int N >
struct place_holder {};

place_holder <1> _1;
place_holder <2> _2;
place_holder <3> _3;
place_holder <4> _4;
place_holder <5> _5;
place_holder <6> _6;
place_holder <7> _7;



template <typename F, typename... Args>
struct bind_function {
    
    typedef std::tuple<std::decay_t<Args>...> my_tuple;

    typename std::decay<F>::type func;
    my_tuple initial_args;
    

    template <typename FF, typename... _Args>
    bind_function(FF&& _func, _Args&& ... args) : func (std::forward<FF>(_func)),
                                            initial_args (std::forward<_Args>(args)...){}


    template < int... S >
    struct sequence 
    {};

    template < int S, int... Rest >
    struct seq_gen {
        typedef typename seq_gen < S - 1, S - 1, Rest... >::type type;
    };


    template < int... Rest >
    struct seq_gen < 0, Rest... > {
        typedef sequence < Rest... > type;
    };




    template <typename initial_arg, typename... new_Args>
    decltype(auto) calc(initial_arg& ar, new_Args&...) {
        return ar;
    }

    template <typename _F, typename... init_Args, typename... new_Args>
    decltype(auto) calc(bind_function <_F, init_Args...>& inner_bind, new_Args&... new_args) {
        return inner_bind(new_args...);
    }
    
    template <int N, typename... new_Args>
    decltype(auto) calc(place_holder <N> &, new_Args&... new_args) {
        return std::get<N>(std::forward_as_tuple(new_args...));
    }

    

    template <typename... new_Args, int... S>
    auto call_with_sequence(const sequence<S...>&, new_Args&&... new_args) {
        return func(calc(std::get<S>(initial_args), new_args...)...);
    }

    template <typename... new_Args>
    auto operator() (new_Args&&... new_args) {
        return call_with_sequence(typename seq_gen<std::tuple_size<my_tuple>::value>::type(), std::forward<new_Args> (new_args)...);    
    }
};



template <typename F, typename ... Args>
bind_function < F, Args... > bind(F&& func, Args&& ... args) {
    return bind_function < F, Args... > (std::forward<F> (func), std::forward<Args> (args)...);
}


double mult(double a, double b, int c) {
    return a * b * c;
}


int main() {

    std::cout << bind(&mult, _2, _2, _1)(7, 0.5) << std::endl;

    return 0;
}