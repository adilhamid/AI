% The Facts Given in the Question
% $Facts about who is whose parent
parent(bart,homer).
parent(bart,marge).
parent(lisa,homer).
parent(lisa,marge).
parent(maggie,homer).
parent(maggie,marge).
parent(homer,abraham).
parent(herb,abraham).
parent(tod,ned).
parent(rod,ned).
parent(marge,jackie).
parent(patty,jackie).
parent(selma,jackie).
%Facts about the Gender
%Female
female(maggie).
female(lisa).
female(marge).
female(patty).
female(selma).
female(jackie).
%Male
male(bart).
male(homer).
male(herb).
male(burns).
male(smithers).
male(tod).
male(rod).
male(ned).
male(abraham).

%Rules to Define the relationships are not included here

%Brother
brother(X,Y):- parent(X,Z),parent(Y,Z),male(Y),X\=Y.

%Sister
sister(X,Y):- parent(X,Z),parent(Y,Z),female(Y),X\=Y.

%Aunt
aunt(X,Y):- parent(X,Z),sister(Z,Y).

%Uncle
uncle(X,Y):- parent(X,Z),brother(Z,Y).

%GrandFather
grandfather(X,Z):- parent(X,Y),parent(Y,Z),male(Z).

%Granddaughter
granddaughter(X,Y):- parent(Y,Z),parent(Z,X),female(Y).

%Ancestor
ancestor(X,Y):- parent(X,Y).
ancestor(X,Y):- parent(X,Z),ancestor(Z,Y).

%related
related(X,Y):-
    ancestor(X, Z),
    ancestor(Y,Z).

%unrelated
unrelated(X,Y):- not(related(X,Y)).

