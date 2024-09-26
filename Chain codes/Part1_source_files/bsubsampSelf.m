function [s, su] = bsubsampSelf(b, gridsep)
%bsubsampSelf Subsample a boundary.

% Check input.
[np, nc] = size(b);
if np < nc 
   error('B must be of size np-by-2.'); 
end
if gridsep/2 ~= round(gridsep/2) 
   error('GRIDSEP must be an even integer.')
end

% Eliminate if the first and last points are same
if isequal(b(1, :), b(np, :))
   np = np - 1;
   b = b(1:np, :);
end

% Find the max x and y
xmax = max(b(:, 1));
ymax = max(b(:, 2));

% Determine the integral number of grid lines
GLx = ceil((xmax + gridsep)/(gridsep + 1));
GLy = ceil((ymax + gridsep)/(gridsep + 1));

% Form vectors of x and y grid locations.
I = 1:GLx;
X(I) = gridsep*I + (I - gridsep); 

J = 1:GLy;
Y(J) = gridsep*J + (J - gridsep); 
DIST = gridsep/2;
[YG, XG] = meshgrid(Y, X);
Q = 1;
for k=1:np
   [I,J] = find(abs(XG - b(k, 1)) <= DIST & abs(YG - b(k, 2)) <= ...
                DIST); 
   I = I(1);
   J = J(1);
   ord = k;
   d1(Q, :) = cat(2, Y(J), ord);
   d2(Q, :) = cat(2, X(I), ord);
   Q = Q + 1;
end
 
d = cat(2, d2(:, 1), d1);
d = fliplr(d); 
d = sortrows(d);
d = fliplr(d); 
s = d(:, 1:2);
[s, m, n] = unique(s, 'rows');
s = [s, m];
s = fliplr(s);
s = sortrows(s);
s = fliplr(s);
s = s(:, 1:2);
su = round(s./gridsep) + 1;