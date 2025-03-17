
% Program: Write a Prolog program to print each element of a list on a new line.
% Motive: To demonstrate two approaches in Prolog:
%         1. Using recursion to process the list.
%         2. Using backtracking with `member/2` to process the list.
% Each approach is explained step by step using comments.

% ---- Approach 1: Recursive Solution ----

% Base Case:
% If the list is empty, do nothing. This stops the recursion.
my_write_recursive([]) :-
    % Comment: Empty list, recursion ends here.
    true.

% Recursive Rule:
% Split the list into head (X) and tail (R), print the head,
% and recursively process the tail.
my_write_recursive([X|R]) :-
    write(X),    % Print the current element.
    nl,          % Move to the next line.
    my_write_recursive(R).  % Recursively process the rest of the list.

% ---- Approach 2: Backtracking Solution ----

% Predicate using backtracking:
% Iterate over all elements of the list using `member/2`,
% print each element, and use `fail` to force backtracking.
my_write_backtrack(L) :-
    % Generate each element of the list using `member/2`.
    member(X, L),
    write(X),     % Print the current element.
    nl,           % Move to the next line.
    fail.         % Force backtracking to explore the next element.

% Ending the predicate:
% This ensures that `true` is printed after all elements have been written.
my_write_backtrack(_) :-
    true.

% ---- How to Run the Program ----
% To test the recursive approach:
% ?- my_write_recursive([a, b, c]).
% Output:
% a
% b
% c
% true.

% To test the backtracking approach:
% ?- my_write_backtrack([a, b, c]).
% Output:
% a
% b
% c
% true.
