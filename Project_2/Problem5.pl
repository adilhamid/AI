%Generate all bit vectors of spefied length (L).
bitvec(L,List):-
    length(B,L),
    make_binary(B),
    copy(B,List).

copy(L,R) :- accCp(L,R).
accCp([],[]).
accCp([H|T1],[H|T2]) :- accCp(T1,T2).

make_binary([]).
make_binary([H|T]) :-
    member(H, [1,0]),
    make_binary(T).

%Counting the number of 1 in the list
occur(_,[],0).
occur(X,[X|T],N) :-
    occur(X,T,NN),
    N is NN + 1.
occur(X,[H|T],N) :-
    occur(X,T,N),
    X \= H.

count(N,List,Cnt):-
    occur(N,List,Cnt).

%Selecting only the vectors with fixed length bits sets.
code(L,S,List):-
    bitvec(L,AllList),
    count(1,AllList,Y),
    Y is S,
    append(AllList,[],List).
