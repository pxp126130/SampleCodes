function [right] = eanRightHandDecoding(rightCode)

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

	right = zeros(1,6);
	rightrearange = reshape(rightCode,7,6);
    rightrearange = rightrearange';


    for i = 1 : 6
        for j = 1 : 10
            if(rightrearange(i,:) == ParityTable(j,:))
                right(i) = j;
            end
        end
    end
    
    right = right - 1;
end