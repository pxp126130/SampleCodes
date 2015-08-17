clear all;
close all;
clc;

b = imread('EESC6363_DIP_ProjectData_Parth/Images/Blurred_Images/filename.jpg');	% Read blur image

k = imread('EESC6363_DIP_ProjectData_Parth/Images/ATC_Corrected/filename.jpg');		% Read atc corrected image

m = imread('EESC6363_DIP_ProjectData_Parth/Images/Proposed_Algo_Results/filename.jpg');	% Read proposed method corrected image

p = imread('EESC6363_DIP_ProjectData_Parth/Images/PSF_Corrected/filename.jpg');		% Read psf corrected image


msek = mse(b,k);				
msem = mse(b,m);
msep = mse(b,p);

display(msek);
display(msem);
display(msep);