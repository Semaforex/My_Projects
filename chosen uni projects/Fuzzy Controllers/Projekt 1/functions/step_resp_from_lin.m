function [S, Sz] = step_resp_from_lin(Ad, Bd, Ed, h1p, h2p, F1p, Fdp, Ts, k_tau)

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
        x_prev = [h1_sim(k-1); h2_sim(k-1)];
        u_prev = F1_sim(k-1-k_tau);
        d_prev = Fd_sim(k-1);
        x_next = Ad * x_prev + Bd * u_prev + Ed * d_prev;

        h1_sim(k) = max(x_next(1), 0);
        h2_sim(k) = max(x_next(2), 0);
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
        x_prev = [h1_sim(k-1); h2_sim(k-1)];
        u_prev = F1_sim(k-1-k_tau);
        d_prev = Fd_sim(k-1);
        x_next = Ad * x_prev + Bd * u_prev + Ed * d_prev;

        h1_sim(k) = max(x_next(1), 0);
        h2_sim(k) = max(x_next(2), 0);
    end

    Sz = h2_sim(k_tau+2:end);