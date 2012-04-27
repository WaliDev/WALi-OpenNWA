#include <string>

std::string answers_forwards_calls[] = {
    //NWA 0
    ""
    ,

    //NWA 1
    "<(program,(q1b,(q0b,q0b))), q0b> -> <program, q0b>\tONE"  "\n"
    "<program, q0b> -> <program, q0b>\tONE"  "\n"
    "<program, q0b> -> <program, q1b q0b>\tONE"  "\n"
    "<(program,(q1b,(q1b,q1b))), q1b> -> <program, q1b>\tONE"  "\n"
    "<program, q1b> -> <program, q1b>\tONE"  "\n"
    "<program, q1b> -> <program, q1b q1b>\tONE"  "\n"
    "<program, q1b> -> <(program,(q1b,(q0b,q0b))), >\tONE"  "\n"
    "<program, q1b> -> <(program,(q1b,(q1b,q1b))), >\tONE"  "\n"
    ,

    //NWA 2
    "<program, q2sr> -> <program, q2sr>\tONE"  "\n"
    "<program, q2sr> -> <program, q3sr q2sr>\tONE"  "\n"
    "<program, q2sr> -> <(program,(q2sr,(q0sr,q2sr))), >\tONE"  "\n"
    "<(program,(q1sr,(q0sr,q0sr))), q0sr> -> <program, q0sr>\tONE"  "\n"
    "<program, q1sr> -> <program, q1sr>\tONE"  "\n"
    "<program, q1sr> -> <program, q1sr q1sr>\tONE"  "\n"
    "<program, q1sr> -> <(program,(q1sr,(q0sr,q0sr))), >\tONE"  "\n"
    "<program, q1sr> -> <(program,(q1sr,(q1sr,q1sr))), >\tONE"  "\n"
    "<(program,(q3sr,(q2sr,q2sr))), q2sr> -> <program, q2sr>\tONE"  "\n"
    "<(program,(q2sr,(q0sr,q2sr))), q0sr> -> <program, q2sr>\tONE"  "\n"
    "<(program,(q1sr,(q1sr,q1sr))), q1sr> -> <program, q1sr>\tONE"  "\n"
    "<(program,(q3sr,(q3sr,q3sr))), q3sr> -> <program, q3sr>\tONE"  "\n"
    "<(program,(q0sr,(q0sr,q2sr))), q0sr> -> <program, q2sr>\tONE"  "\n"
    "<program, q3sr> -> <program, q3sr>\tONE"  "\n"
    "<program, q3sr> -> <program, q3sr q3sr>\tONE"  "\n"
    "<program, q3sr> -> <(program,(q3sr,(q2sr,q2sr))), >\tONE"  "\n"
    "<program, q3sr> -> <(program,(q3sr,(q3sr,q3sr))), >\tONE"  "\n"
    "<program, q0sr> -> <program, q0sr>\tONE"  "\n"
    "<program, q0sr> -> <program, q1sr q0sr>\tONE"  "\n"
    "<program, q0sr> -> <(program,(q0sr,(q0sr,q2sr))), >\tONE"  "\n"
    ,

    //NWA 3
    "<(program,(q1mr,(q1mr,q1mr))), q1mr> -> <program, q1mr>\tONE"  "\n"
    "<(program,(q1mr,(q0mr,q0mr))), q0mr> -> <program, q0mr>\tONE"  "\n"
    "<program, q1mr> -> <program, q1mr>\tONE"  "\n"
    "<program, q1mr> -> <program, q1mr q1mr>\tONE"  "\n"
    "<program, q1mr> -> <(program,(q1mr,(q0mr,q0mr))), >\tONE"  "\n"
    "<program, q1mr> -> <(program,(q1mr,(q1mr,q1mr))), >\tONE"  "\n"
    "<program, q0mr> -> <program, q0mr>\tONE"  "\n"
    "<program, q0mr> -> <program, q1mr q0mr>\tONE"  "\n"
    "<program, q0mr> -> <(program,(q0mr,(q0mr,q0mr))), >\tONE"  "\n"
    "<(program,(q0mr,(q0mr,q0mr))), q0mr> -> <program, q0mr>\tONE"  "\n"
    ,

    //NWA 4
    "<program, q1sl> -> <program, q1sl>\tONE"  "\n"
    "<program, q1sl> -> <program, q1sl q1sl>\tONE"  "\n"
    "<program, q1sl> -> <(program,(q1sl,(q0sl,q0sl))), >\tONE"  "\n"
    "<program, q1sl> -> <(program,(q1sl,(q1sl,q1sl))), >\tONE"  "\n"
    "<(program,(q1sl,(q1sl,q1sl))), q1sl> -> <program, q1sl>\tONE"  "\n"
    "<program, q0sl> -> <program, q0sl>\tONE"  "\n"
    "<program, q0sl> -> <program, q1sl q0sl>\tONE"  "\n"
    "<(program,(q1sl,(q0sl,q0sl))), q0sl> -> <program, q0sl>\tONE"  "\n"
    ,

    //NWA 5
    "<(program,(q1ml,(q0ml,q0ml))), q0ml> -> <program, q0ml>\tONE"  "\n"
    "<program, q1ml> -> <program, q1ml>\tONE"  "\n"
    "<program, q1ml> -> <program, q1ml q1ml>\tONE"  "\n"
    "<program, q1ml> -> <(program,(q1ml,(q0ml,q0ml))), >\tONE"  "\n"
    "<program, q1ml> -> <(program,(q1ml,(q1ml,q1ml))), >\tONE"  "\n"
    "<(program,(q1ml,(q1ml,q1ml))), q1ml> -> <program, q1ml>\tONE"  "\n"
    "<program, q0ml> -> <program, q0ml>\tONE"  "\n"
    "<program, q0ml> -> <program, q1ml q0ml>\tONE"  "\n"
    ,

    //NWA 6
    "<(program,(q1mf,(q1mf,q1mf))), q1mf> -> <program, q1mf>\tONE"  "\n"
    "<program, q0mf> -> <program, q0mf>\tONE"  "\n"
    "<program, q0mf> -> <program, q1mf q0mf>\tONE"  "\n"
    "<program, q0mf> -> <(program,(q0mf,(q0mf,q0mf))), >\tONE"  "\n"
    "<(program,(q0mf,(q0mf,q0mf))), q0mf> -> <program, q0mf>\tONE"  "\n"
    "<program, q1mf> -> <program, q1mf>\tONE"  "\n"
    "<program, q1mf> -> <program, q1mf q1mf>\tONE"  "\n"
    "<program, q1mf> -> <(program,(q1mf,(q0mf,q0mf))), >\tONE"  "\n"
    "<program, q1mf> -> <(program,(q1mf,(q1mf,q1mf))), >\tONE"  "\n"
    "<(program,(q1mf,(q0mf,q0mf))), q0mf> -> <program, q0mf>\tONE"  "\n"
};

