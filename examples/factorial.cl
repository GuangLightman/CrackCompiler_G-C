(**
 * Calculate factorial via recursion
 * Chang Li at UC San Diego
 * Jan. 16, 2016
 *)

class Main {
    
    main() : Object {
        {
        (new IO).out_int(factorial((new IO).in_int()));
        (new IO).out_string("\n");
        }
    };

    factorial(n : Int) : Int {
        if n = 0 then 1 else n * factorial(n-1) fi
    };
};
