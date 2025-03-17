/*
 * It needs to be corrected!!!
File: Puzzles/seating.pro
   Title: Now seating
Suppose 2 boys and 3 girls go to the theater and they all sit on the same row.
The row has only 5 seats.
1. Generate all different ways they can sit in such a way that the boys
are at the end of the row. Count them.
2. Generate all different ways they can sit in such a way that girls
are at the end of the row. Count them.
3. Generate all different ways they can sit in such a way that boys sit
between the girls.
?- findall(S,start1(S),L),length(L,N),write(N).
L = [[b1, g1, g2, g3, b2], [b1, g2, g1, g3, b2], ...]
N = 12
?- findall(S,start2(S),L),length(L,N),write(N).
L = [[g1, g3, b1, b2, g2], [g1, g3, b2, b1, g2], [g1, b1, g3, b2, g2], ...]
N = 24
****************************************************************/

prettyprint([]).
prettyprint([H|T]) :-
    write('Permutation: '), write(H), nl,
    prettyprint(T).

start1(Sol):-
    findall(Perm,
        (Perm = [X,Y,Z,U,V],
        permutation([X,V], [b1,b2]),
        permutation([Y,Z,U], [g1,g2,g3])
        ),
        L),
    prettyprint(L),
    length(L, N),
    write('Total permutations: '), write(N), nl.

start2(Sol):-
    findall(Perm,
            (Perm = [X, Y, Z, U, V],
             member(X, [g1, g2, g3]),
             member(V, [g1, g2, g3]),
             X \= Y,
             member(D, [g1, g2, g3]),
             D \= X,
             D \= Y,
             permutation([Y, Z, U], [b1, b2, D])),
            L),
    prettyprint(L),
    length(L, N),
    write('Total permutations: '), write(N), nl.

start3(Sol):-
    findall(Perm,
        (Perm = [X,Y,Z,U,V],
        permutation([X,Z,V], [g1,g2,g3]),
        permutation([Y,U], [b1,b2])
        ),
        L),
    prettyprint(L),
    length(L, N),
    write('Total permutations: '), write(N), nl.


