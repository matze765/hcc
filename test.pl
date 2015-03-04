

%1
a(X,Y) :- b(X,Z), c(Y,Z).
%2
a(X) :- b(X), c(X).
%3
a(X,Y,Z) :- b(X,Y), c(X,Z).
%4
a(X,Y) :- b(X), c(Y).
%5
a(X) :- b(Y), c(Z). 
%6
a([A|B]) :- b(A), c(B).
%7
a([A],[B]) :- b([[A]]), c([B]).
%8
a(X,Y,Z) :- b(X,Y), c(X,Z), d(X).

%9
a(X,Y) :- X>Y, X>=Y, X<Y, X<=Y, X = Y, X==Y, X\=Y, X\==Y.



