function [S, Sz] = step_resp(h1p, h2p, F1p, Fdp, Ts, k_tau, C1, C2, alpha1, alpha2)

    dF1 = 1;
    dFd = 1;
    Tsim = 70000;
    kk = floor(Tsim/Ts);
    h1_sim = ones(1, kk)*h1p;
    h2_sim = ones(1, kk)*h2p;
    F1_sim = ones(1, kk)*F1p;
    Fd_sim = ones(1, kk)*Fdp;

    F1_sim(k_tau+2:kk) = F1p+dF1;

    for k = k_tau+2:kk
        h1_sim(k) = h1_sim(k-1) + Ts * (1/(2*C1*h1_sim(k-1))) * ...
            (F1_sim(k-1-k_tau) + Fd_sim(k-1) - alpha1*sqrt(h1_sim(k-1)));
        
        h2_sim(k) = h2_sim(k-1) + Ts * (1/(3*C2*(h2_sim(k-1))^2)) * ...
            (alpha1*sqrt(h1_sim(k-1)) - alpha2*sqrt(h2_sim(k-1)));

        h1_sim(k) = max(h1_sim(k), 0);
        h2_sim(k) = max(h2_sim(k), 0);
    end

    S = h2_sim(k_tau+2:end);
    S = S-h2p;
    
    Tsim = 70000;
    kk = floor(Tsim/Ts);

    h1_sim = ones(1, kk)*h1p;
    h2_sim = ones(1, kk)*h2p;
    F1_sim = ones(1, kk)*F1p;
    Fd_sim = ones(1, kk)*Fdp;

    Fd_sim(k_tau+2:kk) = Fdp+dFd;

    for k = k_tau+2:kk
        h1_sim(k) = h1_sim(k-1) + Ts * (1/(2*C1*h1_sim(k-1))) * ...
            (F1_sim(k-1-k_tau) + Fd_sim(k-1) - alpha1*sqrt(h1_sim(k-1)));
        
        h2_sim(k) = h2_sim(k-1) + Ts * (1/(3*C2*(h2_sim(k-1))^2)) * ...
            (alpha1*sqrt(h1_sim(k-1)) - alpha2*sqrt(h2_sim(k-1)));

        h1_sim(k) = max(h1_sim(k), 0);
        h2_sim(k) = max(h2_sim(k), 0);
    end

    Sz = h2_sim(k_tau+2:end);
    Sz = Sz-h2p;
end