function  c = fchcodeSelf(b, conn, dir)
%fchcodeSelf Computes the Freeman chain code of a boundary.

% Preliminaries.
if nargin == 1 
   dir = 'same'; 
   conn = 8;
elseif nargin == 2 
   dir = 'same';
elseif nargin == 3   
   % Nothing to do here.
else 
   error('Incorrect number of inputs.')
end
[np, nc] = size(b);
if np < nc 
   error('B must be of size np-by-2.'); 
end
 
if isequal(b(1, :), b(np, :))
   np = np - 1;
   b = b(1:np, :);
end

C(11)=0; C(7)=1; C(6)=2; C(5)=3; C(9)=4;
C(13)=5; C(14)=6; C(15)=7;

x0 = b(1, 1);
y0 = b(1, 2);
%first position of code
c.x0y0 = [x0, y0];
a = circshift(b, [-1, 0]);
DEL = a - b;
if any(abs(DEL(:, 1)) > 1) | any(abs(DEL(:, 2)) > 1);
   error('The input curve is broken or points are out of order.')
end
z = 4*(DEL(:, 1) + 2) + (DEL(:, 2) + 2);
fcc = C(z);
if strcmp(dir, 'reverse')
   fcc = coderev(fcc); % See below for function coderev.
end
% 4 point connectivity
if conn == 4
   val = find(fcc == 1 | fcc == 3 | fcc == 5 | fcc ==7 );
   if isempty(val)
      fcc = fcc./2;
   else
      warning('The specified 4-connected code cannot be satisfied.')
   end
end

c.fcc = fcc;
% first difference of fcc.
c.diff = codediff(fcc,conn);
% integer of minimum magnitude.
c.mm = minmag(fcc);
% first difference of fcc
c.diffmm = codediff(c.mm, conn);

%-------------------------------------------------------------------%
function cr = coderev(fcc)
cr = fliplr(fcc);
ind1 = find(0 <= cr & cr <= 3);
ind2 = find(4 <= cr & cr <= 7);
cr(ind1) = cr(ind1) + 4;
cr(ind2) = cr(ind2) - 4;
%-------------------------------------------------------------------%
function z = minmag(c)
%minmag Finds the integer of minimum magnitude in a chain code.
I = find(c == min(c));
J = 0;
A = zeros(length(I), length(c));
for k = I;
   J = J + 1;
   A(J, :) = circshift(c,[0 -(k-1)]);
end
 
[M, N] = size(A);
J = (1:M)';
for k = 2:N
   D(1:M, 1) = Inf;
   D(J, 1) = A(J, k);
   amin = min(A(J, k));
   J = find(D(:, 1) == amin);
   if length(J)==1
      z = A(J, :);
      return
   end
end
%-------------------------------------------------------------------%
function d = codediff(fcc, conn)
%codediff Computes the first difference of a chain code.
sr = circshift(fcc, [0, -1]); % Shift input left by 1 location.
delta = sr - fcc;
d = delta;
I = find(delta < 0);
 
type = conn;
switch type
case 4 % Code is 4-connected
   d(I) = d(I) + 4;
case 8 % Code is 8-connected
   d(I) = d(I) + 8;
end
