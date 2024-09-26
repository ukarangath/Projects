%-------------------------------------------------------
%Reading given image
I = imread('Figure1.tif');
figure,imshow(I);title('Initial Image');
%-------------------------------------------------------
w = fspecial('average',9);
%Smoothing with 9x9 averaging filter with 'replicate' option
g = imfilter(I,w,'replicate');
figure,imshow(g);title('Smoothed image');
%-------------------------------------------------------
gB = im2bw(g, 0.5);
figure,imshow(gB);title('Image after thresholding');
%-------------------------------------------------------
%Finding the outer boundaries
B = bwboundaries (gB, 'noholes');
d = cellfun('length', B);
[maxd, k] = max (d) ;
b = B{k};
[M N] = size(g);
%Given function - bound2im
g = bound2im(b, M, N);
figure,imshow(g);title('g : Outer boundary'); 
%-------------------------------------------------------
[s, su] = bsubsampSelf(b, 50);
g2 = bound2im(s, M, N);
figure,imshow(g2);title('g2 : Subsampled boundary'); 
%-------------------------------------------------------
cn = connectpolySelf(s(:, 1), s(:, 2));
g3 = bound2im(cn, M, N);
figure,imshow(g3);title('g3 : Connected sequence from subsampled boundary'); 
%-------------------------------------------------------
C = fchcodeSelf(su,4);
disp(C.x0y0);
disp(C.fcc);
disp(C.mm);
disp(C.diff);
disp(C.diffmm);
%-------------------------------------------------------