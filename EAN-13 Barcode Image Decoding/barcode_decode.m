%barcode_decode

clc;
clear all;
close all;


x = imread('BCode.png');
x = double(x);
[r,c] = size(x);

y = x(floor(r/2),:);
y = 1-y;

for i = 1 : length(y)
    if(y(i)==1)
        break;
    end
end

y = y(i:end);
y = y(end:-1:1);

for i = 1 : length(y)
    if(y(i)==1)
        break;
    end
end

y = y(i:end);
y = y(end:-1:1);


leftCode = y(4:45);
rightCode = y(51:end-3);

[left,parity] = eanLeftHandDecoding(leftCode);
digit = eanFirstDigitDecoding(parity);
right = eanRightHandDecoding(rightCode);


decoded = [digit left right];
