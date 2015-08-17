function [ Y,dm,dv ] = atcdeblur(blur,lowe,nm,nv)

% This function performs deblur using Adaptive Tonal Correction


hsvb = rgb2hsv(blur);							   % Converting blur image to HSI 
xb = hsvb(:,:,3);
xb = xb/max(xb(:));							   % Nonrmalizing the Intensity component to operate upon

hsvl = rgb2hsv(lowe);							   % Converting low exposure image to HSI 
xl = hsvl(:,:,3);
xl = xl/max(xl(:));							   % Nonrmalizing the Intensity component to operate upon

hsvt = hsvl;

mb = mean(xb(:));     
vb = myvar(xb);                                                             
[r c] = size(xb);


cnt=1;                                                                     % counter to accummulate various hits

for a = 1 : 70
    for b = 1 : 30
        for i = 1 : r
            for j = 1 : c
                f(i,j) = log((a-1)*xl(i,j)+1)/log(a);                       % manipulating the mean
                g(i,j) = (atan(b*(f(i,j)-0.5)))/((2*atan(b/2))+0.5);        % manipulating the contrast based on 'f'
            end
        end
        g = g-min(g(:));                                                   % changing the range to positive values
        g = g/max(g(:));                                                   % normalising the image
        mg = mean(g(:));                                                   % mean of transformed image
        vg = myvar(g);                                                     % variance of transformed image
        diff_m = abs(mb-mg)*100/mb;                                        % percentage error in mean
        diff_v = abs(vb-vg)*100/vb;                                        % percentage error in variance
        if(diff_m <= nm)                                                   % limiting the error in mean
          if(diff_v <= nv)                                                 % limiting the error in variance 
             y(:,:,cnt) = g;                                               % storing the hits in an array
             d(cnt,1) = diff_m;                                            % creating a 2D vector
             d(cnt,2) = diff_v;                                            % to store different
             d(cnt,3) = a;                                                 % parameters
             d(cnt,4) = b;                                                 % of all hits
             cnt = cnt+1;
          end
        end
    end
end

vmin = min(d(:,2));                                                         % minimum value of percentage error in variance amongst all hits

for i = 1 : cnt-1                                                           % extracting image that has the minimum percentage error in variance
    if(d(i,2) == vmin)
            I = i;
    end
end

hsvt(:,:,3) = y(:,:,I);							    % replacing the transformed intensity component back to the HSI space.
dm = d(I,1);
dv = d(I,2);

Y = hsv2rgb(hsvt);							   % converting back to RGB


end