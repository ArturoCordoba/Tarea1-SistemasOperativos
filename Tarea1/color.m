clc; clear; close all;
pkg load image;

A = imread('imagenes/person.png');
[m,n,r] = size(A);


sumR = sum(A(:,:,1)(:));
sumG = sum(A(:,:,2)(:));
sumB = sum(A(:,:,3)(:));

max_r = max(max(sumR,sumG),sumB);

r = 0;
if max_r == sumR r = 1; endif
if max_r == sumG r = 2; endif
if max_r == sumB r = 3; endif

% r = 1 RED
% r = 2 GREEN
% r = 3 BLUE 
r