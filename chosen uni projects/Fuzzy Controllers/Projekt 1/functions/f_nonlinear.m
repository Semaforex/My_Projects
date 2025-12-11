function dx = f_nonlinear(t,x,Fin_fun,Fd_fun,tau,C1,C2,alpha1,alpha2)
    h1 = max(x(1), 1e-9);
    h2 = max(x(2), 1e-9);

    
    Fin_tau = Fin_fun(t - tau);
    Fd      = Fd_fun(t);

    dh1 = (1/(2*C1*h1)) * ( Fin_tau + Fd - alpha1*sqrt(h1) );
    dh2 = (1/(3*C2*h2^2)) * ( alpha1*sqrt(h1) - alpha2*sqrt(h2) );

    dx  = [dh1; dh2];
end
