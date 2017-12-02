%Facts for the State Description
% Please make sure that the state description is entered in Row Major
% fashion i.e p(x,R,C) -> where R is row and C is column

p(x,1,1).
p(x,1,2).
p(o,1,3).
p(o,2,1).
p(o,2,3).


%The above configuration can be depicted as follows
%		C1 C2 C3
%
% R1:   X  X  O  
% R2:   O  -  O 
% R3:   -  -  -
%


%PossibleValue
val(1).
val(2).
val(3).

%Utility Functions
blank(R,C):-
    not(p(x,R,C)), not(p(o,R,C)).

different([]).
different([H|T]):-
    not(member(H,T)),different(T).

%Prdicates for the move y the player
ttt_move(x,C,R):-
    val(R),val(C),
    canWin(x,C,R),
    write("Go For Win! (x)").

ttt_move(x,R,C):-
    val(R),val(C),
    canWin(o,R,C),not(canWin(x)),
    write("Move to Block Opponent (o) !").

ttt_move(o,R,C):-
    val(R),val(C),
    canWin(o,R,C),
    write("Go For Win! (o)").


ttt_move(o,R,C):-
    val(R),val(C),
    canWin(x,R,C),not(canWin(o)),
    write("Move to Block Opponent (x) !").

canWin(P,R,C):-
    twoInRow(P,R,C).

canWin(P,R,C):-
    twoInCol(P,R,C).

canWin(P,R,C):-
    twoInDiag(P,R,C).

canWin(P):-
    val(R),val(C),
    canWin(P,R,C).

twoInRow(P,R,C3):-
    val(C1),val(C2),
    p(P,R,C1), p(P,R,C2), blank(R,C3),
    different([C1,C2,C3]).

twoInCol(P,R3,C):-
    val(R1),val(R2),
    p(P,R1,C), p(P,R2,C), blank(R3,C),
    different([R1,R2,R3]).

equal(R,C):-
    R is C.

equal(R,C,Num):-
    R is Num,
    C is Num.

isMainDiag(R1,C1,R2,C2,R,C):-
    different([R1,R2,R]) , different([C1,C2,C]),
    equal(R1,C1),
    equal(R2,C2),
    equal(R,C).

isOffDiag(R1,C1,R2,C2,R3,C3):-
     different([R1,R2,R3]), different([C1,C2,C3]),
     equal(R1,C1,2),
     Val1 is abs(R2-C2),
     Val2 is abs(R3-C3),
     equal(Val1,2),
     equal(Val2,2).

isOffDiag(R1,C1,R2,C2,R3,C3):-
     different([R1,R2,R3]), different([C1,C2,C3]),
     equal(R2,C2,2),
     Val1 is abs(R1-C1),
     Val2 is abs(R3-C3),
     equal(Val1,2),
     equal(Val2,2).

isOffDiag(R1,C1,R2,C2,R3,C3):-
     different([R1,R2,R3]), different([C1,C2,C3]),
     equal(R3,C3,2),
     Val1 is abs(R1-C1),
     Val2 is abs(R2-C2),
     equal(Val1,2),
     equal(Val2,2).

isDiag(R,C,R1,C1,R2,C2):-
    isMainDiag(R1,C1,R2,C2,R,C).

isDiag(R,C,R1,C1,R2,C2):-
    isOffDiag(R1,C1,R2,C2,R,C).

twoInDiag(P,R,C):-
    val(R1),val(R2),val(C1),val(C2),
    p(P,R1,C1), p(P,R2,C2), blank(R,C),
    isDiag(R,C,R1,C1,R2,C2).







