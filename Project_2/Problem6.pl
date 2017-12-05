%Implementation of Sin_Zero
threshold(Val):-
   % write("THRESHOLD: "),writeln(Val),
    Val >= -0.0001, Val =< 0.0001.

update(X,Y):-
    S is sin(X), C is cos(X),
    D is S/C,
    NX is X - D,
   % writeln("Updating the Value mate: "),
   % writeln(X), writeln(D), writeln(S), writeln(C), writeln(NX),
    newtonM(NX,Y).

thresholdCheck(X):-
    Y is sin(X), threshold(Y).

newtonM(X,Y):-
    Y is X,
    thresholdCheck(Y).

newtonM(X,Y):-
    update(X,Y).

sin_zero(X,Y):-
    newtonM(X,Y), format("Y = ~4f",[Y]).
