function s = ifourierdescp(z, nd)
%ifourierdescp Computes inverse Fourier descriptors.
np = length(z);
if nargin == 1 | nd > np 
   nd = np; 
end
x = 0:(np - 1);
m = ((-1) .^ x)';
d = round((np - nd)/2); 
z(1:d) = 0;
z(np - d + 1:np) = 0;
zz = ifft(z);
s(:, 1) = real(zz);
s(:, 2) = imag(zz);
s(:, 1) = m.*s(:, 1);
s(:, 2) = m.*s(:, 2);