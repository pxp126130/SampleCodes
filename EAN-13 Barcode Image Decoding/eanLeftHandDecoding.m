function [left,parity] = eanLeftHandDecoding(leftCode)

    ParityTable(1,:,1) = [0,0,0,1,1,0,1];
    ParityTable(1,:,2) = [0,0,1,1,0,0,1];
    ParityTable(1,:,3) = [0,0,1,0,0,1,1];
    ParityTable(1,:,4) = [0,1,1,1,1,0,1];
    ParityTable(1,:,5) = [0,1,0,0,0,1,1];
    ParityTable(1,:,6) = [0,1,1,0,0,0,1];
    ParityTable(1,:,7) = [0,1,0,1,1,1,1];
    ParityTable(1,:,8) = [0,1,1,1,0,1,1];
    ParityTable(1,:,9) = [0,1,1,0,1,1,1];
    ParityTable(1,:,10) = [0,0,0,1,0,1,1];

    ParityTable(2,:,1) = [0,1,0,0,1,1,1];
    ParityTable(2,:,2) = [0,1,1,0,0,1,1];
    ParityTable(2,:,3) = [0,0,1,1,0,1,1];
    ParityTable(2,:,4) = [0,1,0,0,0,0,1];
    ParityTable(2,:,5) = [0,0,1,1,1,0,1];
    ParityTable(2,:,6) = [0,1,1,1,0,0,1];
    ParityTable(2,:,7) = [0,0,0,0,1,0,1];
    ParityTable(2,:,8) = [0,0,1,0,0,0,1];
    ParityTable(2,:,9) = [0,0,0,1,0,0,1];
    ParityTable(2,:,10) = [0,0,1,0,1,1,1];
    
    left = zeros(1,6);
    parity = zeros(1,6);
    leftrearange = reshape(leftCode,7,6);
    leftrearange = leftrearange';
    
    for k = 1 : 6
        for i = 1 : 2
            for j = 1 : 10
                if(leftrearange(k,:) == ParityTable(i,:,j))
                    parity(k) = i;
                    left(k) = j;
                end
            end
        end
    end
    
    left = left - 1;
end