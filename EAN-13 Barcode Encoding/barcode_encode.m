% Creation of 1D EAN-13 Barcode
clc;
clear all;
close all;


buf = zeros(1,8);
sentinel = [1,0,1];
centerguard = [0,1,0,1,0];

codenum = input('Enter 12 digits to be encoded: ');

code = eanCreateCodeArray(codenum);
check = eanCheckDigit(code);

eanCode = [code check];

display(code);
display(check);
display(eanCode);

parity = eanLeftHandCodingParity(eanCode);
left = eanLeftHandEncoding(parity,eanCode);
right = eanRightHandEncoding(eanCode);

bar = [buf sentinel left centerguard right sentinel buf];
bar2 = [buf sentinel zeros(1,length(left)) centerguard zeros(1,length(right)) sentinel buf];

barCode = zeros(70,length(bar));

for i = 1 : 65
    barCode(i,:) = bar;
end

for i = 66 : 70
    barCode(i,:) = bar2;
end
imshow(1-barCode);

