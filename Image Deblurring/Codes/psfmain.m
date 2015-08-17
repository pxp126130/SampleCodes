clear all;
close all;
clc;

blur = imread('EESC6363_DIP_ProjectData_Parth/Images/Blurred_Images/filename.jpg');	% Read blur image

data = 'EESC6363_DIP_ProjectData_Parth/Sensor_Data/beats.xlsx';				% Read inertial sensor data
x_acc = xlsread(data,'B9:B36');								% isolate accelerometer data in x
y_acc = xlsread(data,'C9:C36');								% isolate accelerometer data in y

mul = input('Enter multiplier constant for "f/z": ');					% constant multiplier
isnr = input('Enter value of inverse SNR: ');						% inverse signal to noise ratio for blind deconvolution


[y,psf] = psfdeblur(blur,x_acc,y_acc,mul,isnr);

figure
imshow(blur)
title('blurred image');

figure
imshow(psf)
title('PSF Mask');

figure
imshow(y)
title('deblurred image');

imwrite(y,'beatsp.jpg');								% Data written in the bin folder of matlab in the C drive