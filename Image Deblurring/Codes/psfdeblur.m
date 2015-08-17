function [y,psf1] = psfdeblur(blur,x_acc,y_acc,mul,isnr)

xm = mean(x_acc);					% acceleration data parameters passed from main code
ym = mean(y_acc);

x_vel = cumtrapz(x_acc - xm);				% first integration to obtain velocity vector
y_vel = cumtrapz(y_acc - ym);

x_pos1 = cumtrapz(x_vel);				% second integration to obtain position vector
y_pos1 = cumtrapz(y_vel);

x_pos = round(mul*(x_pos1 - min(x_pos1)))+1;		% manipulating the position vector obtained above
y_pos = round(mul*(y_pos1 - min(y_pos1)))+1;		% to yeild a vector with range starting from 1

psf1 = zeros();
for i = 1 : length(x_pos)
    psf1(x_pos(i),y_pos(i)) = 1;			% generate PSF mask
end

s=sum(psf1(:));
psf=psf1/s;						% normalize PSF mask

hsvb = rgb2hsv(blur);					% convert image to HSI space
yt = deconvwnr(hsvb(:,:,3),psf,isnr);			% perform wiener blind deconvolution using
												% isnr data passed as parameter
hsvy = hsvb;
hsvy(:,:,3) = yt;					% replace original intensity plane by new deblurred plane
y = hsv2rgb(hsvy);					% convert back to RGB space
	
end