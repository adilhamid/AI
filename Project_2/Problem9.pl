% State Description
% Please make sure that the state description is entered in Row Major
% fashion i.e p(C,R) -> where R is row and C is column
% Facts about the Wompus World

visited(4,1).
visited(4,2).
visited(4,3).
visited(4,4).
stench(4,2).
breeze(4,3).
breeze(4,4).

%The above configuration can be depicted as follows
%	C1 C2 C3 C4
% R1:   -  -  P   V
% R2:   -  -  P   V
% R3:   -  G  W   V
% R4:   -  -  -   V
%

%Possible Values for R and C
val(1).
val(2).
val(3).
val(4).

pitFree(X,Y):-
    val(X),val(Y),
    val(X1),val(Y1),
    adjacent(X,Y,X1,Y1),
    not(visited(X,Y)),
    not(breeze(X1,Y1)).

wompusFree(X,Y):-
    val(X),val(Y),
    val(X1),val(Y1),
    not(stench(X1,Y1)),
    adjacent(X,Y,X1,Y1),
    not(visited(X,Y)).

adjacent(X1,Y1,X,Y):-
    X is X1+1,
    Y is Y1+0,
    val(X),val(Y).

adjacent(X1,Y1,X,Y):-
    X is X1,
    Y is Y1+1,
    val(X),val(Y).

adjacent(X1,Y1,X,Y):-
    X is X1-1,
    Y is Y1+0,
    val(X),val(Y).

adjacent(X1,Y1,X,Y):-
    X is X1,
    Y is Y1-1,
    val(X),val(Y).

candidate(X,Y):-
    val(X), val(Y),
    val(X1), val(Y1),
    visited(X1,Y1),
    not(visited(X,Y)),adjacent(X1,Y1,X,Y).

ww_move(X,Y):-
    val(X),val(Y),
    wompusFree(X,Y),
    pitFree(X,Y),
    candidate(X,Y).








