function [check] = eanCheckDigit(code)

% This function calculates the check digit for encoding a 1-D EAN-13 barcode


    weights = [1,3,1,3,1,3,1,3,1,3,1,3];

    y = code.*weights;

    sumy = sum(y);

    check = 10-mod(sumy,10);

end

