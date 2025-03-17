% Program: Using append/3 to implement my_set_minus/3
% Motive: This predicate demonstrates how to concatenate or split lists using Prolog's built-in `append/3`.
%         The name `my_set_minus` may be misleading, as it does not perform set subtraction.
%         Instead, it ensures that the concatenation of list `Y` and list `Z` gives list `X`.
%
% Predicate: my_set_minus(X, Y, Z)
% Parameters:
%   - X: The resulting list (formed by appending Y and Z).
%   - Y: The first part of the list.
%   - Z: The second part of the list.

% Rule: Use append/3 to define my_set_minus/3
my_set_minus(X, Y, Z) :-
    append(Y, Z, X).

% ---- Examples ----

% Example 1: Splitting a list into two parts
% Query: ?- my_set_minus([1, 2, 3, 4], [1, 2], Z).
% Explanation:
%   - X = [1, 2, 3, 4]: The original list.
%   - Y = [1, 2]: The first part of the list.
%   - Z = [3, 4]: The second part of the list.
% Result: Z = [3, 4].
%
% To test:
% ?- my_set_minus([1, 2, 3, 4], [1, 2], Z).

% Example 2: Extracting the first part of a list
% Query: ?- my_set_minus([a, b, c, d], Y, [c, d]).
% Explanation:
%   - X = [a, b, c, d]: The original list.
%   - Y = [a, b]: The first part of the list.
%   - Z = [c, d]: The second part of the list.
% Result: Y = [a, b].
%
% To test:
% ?- my_set_minus([a, b, c, d], Y, [c, d]).

% Example 3: Concatenating two lists
% Query: ?- my_set_minus(X, [x, y], [z]).
% Explanation:
%   - Y = [x, y]: The first part of the list.
%   - Z = [z]: The second part of the list.
%   - X = [x, y, z]: The result of appending Y and Z.
% Result: X = [x, y, z].
%
% To test:
% ?- my_set_minus(X, [x, y], [z]).

% Example 4: Checking if one list is the concatenation of two others
% Query: ?- my_set_minus([p, q, r, s], [p], [q, r, s]).
% Explanation:
%   - X = [p, q, r, s]: The original list.
%   - Y = [p]: The first part of the list.
%   - Z = [q, r, s]: The second part of the list.
% Result: true.
%
% To test:
% ?- my_set_minus([p, q, r, s], [p], [q, r, s]).


