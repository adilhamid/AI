%Send More Money Problem
digit(0).
digit(1).
digit(2).
digit(3).
digit(4).
digit(5).
digit(6).
digit(7).
digit(8).
digit(9).

different([]).
different([H|T]):-
    not(member(H,T)),different(T).
sendMoreMoney:-
    digit(S), digit(E), digit(N), digit(D),digit(M), digit(O),digit(R), digit(N), digit(Y),
    M is 1+0, S>7,N is E+1, % Problem Statement Heuristics
    SEND is 1000*S+100*E+10*N+D,
    MORE is 1000*M+100*O+10*R+E,
    MONEY is 10000*M + 1000*O + 100*N + 10*E + Y,
    MONEY is SEND + MORE,
    different([S,E,N,D,M,O,R,Y]),
    print([[S, E, N, D], [M, O, R, E], [M, O, N, E, Y]]).
