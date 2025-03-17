% The following program is to test for debugger.

parent(john, mary).
parent(mary, alice).

ancestor(X, Y) :- parent(X, Y).  % Base case: A parent is an ancestor.
ancestor(X, Y) :-               % Recursive case:
    parent(X, Z),
    ancestor(Z, Y).
