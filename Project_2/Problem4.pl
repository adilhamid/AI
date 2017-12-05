%Problem 4
%Prime Factorization in Prolog
%
factor(N,M):-
    factorize(N,2,[],M).

%Base Cases
factorize(1,_,M,M).

%Main Function to call the factorization function
factorize(N,Count,Acc, M):-
    divisible(N,Count),Count =< N,
    X is N/Count,
    factorize(X,Count,[Count|Acc],M).

factorize(N,Count,Acc,M):-
    Count =< N,NewCount is Count+1,
    factorize(N,NewCount,Acc,M).

factorize(N,Count,Acc,M):-
     Count>=N,
     factorize(1,Count,Acc,M).

divisible(N,X):-
    M is N mod X, M=0.
