function [L, K] = parametry_obserwator_regulator(s_b, s_o_val, A, B, C)

    syms L1 L2 L3 L4 s s_o
    L = [L1; L2; L3; L4];
    Mat = s*eye(4) - A + L * C;
    det_eq = det(Mat);

    desired_char_eq = expand((s - s_o)^4);

    char_coeffs = coeffs(det_eq, s, 'All'); 
    desired_coeffs = coeffs(desired_char_eq, s, 'All');

    eqns = char_coeffs == desired_coeffs;
    sol = solve(eqns, [L1, L2, L3, L4]);

    L_sol = [sol.L1; sol.L2; sol.L3; sol.L4];
    L = double(subs(L_sol, s_o, s_o_val));


    K = acker(A, B, [s_b s_b s_b s_b]);
end