function [K, K_e] = parametry_regulator_calkowy(A, B, C, s_b)
    n = size(A, 1);   

    A_r = [A, zeros(n, 1); -C, 0];
    B_r = [B; 0];

    desired_poles = s_b * ones(1, n + 1);

    KK_e = acker(A_r, B_r, desired_poles);
    K = KK_e(1, 1:n);    
    K_e = KK_e(1, n+1);  
end
