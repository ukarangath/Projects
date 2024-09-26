%-------------------------------------------------------
%Reading given image
I = imread('Figure2.tif');
figure,imshow(I);title('Initial Image');
%-------------------------------------------------------
b = bwboundaries (I,'noholes') ;
b = b{1};
bim = bound2im(b, size(I, 1), size(I, 2));
figure,imshow(bim);title('Extracted boundary');
%-------------------------------------------------------
z = fourierdescp(b);
%Reconstructing using 50% of possible descriptors
s546 = ifourierdescp(z, 546);
s546im = bound2im(s546, size(I, 1), size(I, 2));
figure,imshow(s546im);title('Reconstructed boundary - 50%');
%Reconstructing using 1%(11) of possible descriptors
s11 = ifourierdescp(z, 11);
s11im = bound2im(s11, size(I, 1), size(I, 2));
figure,imshow(s11im);title('Reconstructed boundary - 1%');
%-------------------------------------------------------