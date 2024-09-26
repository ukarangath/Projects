function c = connectpolySelf(x, y)
%connectpolySelf Connects vertices of a polygon.

v = [x(:), y(:)];
if ~isequal(v(end, :), v(1, :))
   v(end + 1, :) = v(1, :);
end
% Connect vertices.
segments = cell(1, length(v) - 1);
for I = 2:length(v)
   [x, y] = intline(v(I - 1, 1), v(I, 1), v(I - 1, 2), v(I, 2));
   segments{I - 1} = [x, y];
end
c = cat(1, segments{:});