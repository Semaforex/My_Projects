function F = genF1trajectory(F1min, F1max, F1start, n)


    if F1min >= F1max
        error('Musi być F1min < F1max.');
    end
    if F1start < F1min || F1start > F1max
        error('F1start musi być w [F1min, F1max].');
    end

    % liczba punktów F1
    M = n + 1;

    % siatka wartości F1 rozłożonych równomiernie
    levels = linspace(F1min, F1max, M);
    F = levels(randperm(length(levels))); 

    F = F(:)';
    F(1) = F1start;

    
end
