%Facts for the State Description
p(x,1,1).
p(x,1,3).
p(o,3,1).
p(o,1,3).

%Opposite Player
notmember(x,OP):-
   OP is o.

notmember(o,OP):-
   OP is x.

%Prdicates for the move y the player
ttt_move(P,R,C):-
    CP is P,
    notmember(P,OP),
    writeln(CP), writeln(OP),
    writeln(R), writeln(C).
    % canWin(CP,R,C),move(CP,R,C),not(canWin(o,R,C)).


%canWin(P,R,C):-
 %   twoInRow(P,R).
%canWin(P,R,C):-
 %   twoInCol(P,C).
%canWin(P,R,C):-
 %   twoInDiag(P,R,C).


