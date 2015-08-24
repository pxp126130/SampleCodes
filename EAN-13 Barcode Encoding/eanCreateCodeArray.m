function [code] = eanCreateCodeArray(codenum)

% This function converts a regular number into an array for the 1-D barcode

% This function automatically appends zeros in the front if there are less
% than 11 digits entered.

% This function also truncates any digits entered after the first 12
% digits.

    l = numel(num2str(codenum));

    codetemp = zeros(1,l);
    for i = 1 : l
        temp = floor(codenum/(10^(i-1)));
        codetemp(l+1-i) = mod(temp,10);
    end

    if (l > 12)
        code = codetemp(1:12);
    elseif (l < 12)
        app = zeros(1,12-l);
        code = [app codetemp];
    else
        code = codetemp;
    end
    
end

