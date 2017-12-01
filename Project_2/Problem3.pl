
%remove Duplicates
remdups(List,Ans):-
     remove(List, [], Ans).

remove([],Ans,Ans).

remove([H | T],Acc, Ans) :-
     member(H,Acc),
     remove( T,Acc, Ans).

remove([H | T],Acc,Ans) :-
      remove( T, [H|Acc],Ans).
