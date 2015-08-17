clear all;
close all;
clc;

blur = imread('D:/P_UTD/SP 13/DIP/EESC6363_DIP_ProjectData_Parth/Images/Blurred_Images/5ftb.jpg');	% Read blur image
lowe = imread('D:/P_UTD/SP 13/DIP/EESC6363_DIP_ProjectData_Parth/Images/Low_Exposure/5ftl.jpg');	% Read Low expoure image

nm=input('specify tolerance for mean in %: ');                             		% User input for tolerance on percentage error in mean
nv=input('specify tolerance for variance in %: ');                          		% User input for tolerance on percentage error in variance

[atc,dm,dv] = atcdeblur(blur, lowe, nm, nv);

imwrite(atc,'output.jpg');

figure
imshow(atc)
title('Deblurred Image');
figure
imshow(blur)
title('Blur Image');
display(dm)
display(dv)
