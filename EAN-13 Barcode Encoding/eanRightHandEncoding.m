function rightCode = eanRightHandEncoding(eanCode)

% This code returns the bit pattern for the right hand-side code based on
% the parity calculated from the eanrightHandCodingParity function and the
% eanCode to be encoded itself.

    rightCode = [];

    ParityTable(1,:) = [1,1,1,0,0,1,0];
    ParityTable(2,:) = [1,1,0,0,1,1,0];
    ParityTable(3,:) = [1,1,0,1,1,0,0];
    ParityTable(4,:) = [1,0,0,0,0,1,0];
    ParityTable(5,:) = [1,0,1,1,1,0,0];
    ParityTable(6,:) = [1,0,0,1,1,1,0];
    ParityTable(7,:) = [1,0,1,0,0,0,0];
    ParityTable(8,:) = [1,0,0,0,1,0,0];
    ParityTable(9,:) = [1,0,0,1,0,0,0];
    ParityTable(10,:) = [1,1,1,0,1,0,0];

    right = eanCode(8:end);

    for i = 1 : length(right)
        rightCode = [rightCode ParityTable(right(i)+1,:)];
    end
    
end

