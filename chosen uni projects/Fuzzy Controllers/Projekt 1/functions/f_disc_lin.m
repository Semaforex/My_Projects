function [Ad, Bd, Ed, h1bar, h2bar, F1bar, Fdbar] = f_disc_lin(C1, C2, alpha1, alpha2, varbar, Fdbar, Ts, calc_h)
    if calc_h
        F1bar = varbar;
        h1bar = 1/alpha1^2*(F1bar+Fdbar)^2;
        h2bar = alpha1^2/alpha2^2*h1bar;
    else
        h2bar = varbar;
        h1bar = h2bar*alpha2^2/alpha1^2;
        F1bar = sqrt(h1bar*alpha1^2);
        Fdbar = 0;
    end
    
    df1_dh1 = -(1/(2*C1))*( alpha1/(2*h1bar*sqrt(h1bar)) ...
                + (F1bar+Fdbar-alpha1*sqrt(h1bar))/h1bar^2 );
    df1_dh2 = 0;
    df1_dF1 =  1/(2*C1*h1bar);
    df1_dFd =  1/(2*C1*h1bar);
    
    df2_dh1 =  alpha1/(6*C2*sqrt(h1bar)*h2bar^2);
    df2_dh2 =  (1/(3*C2))*( -alpha2/(2*h2bar^2*sqrt(h2bar)) ...
                - 2*(alpha1*sqrt(h1bar)-alpha2*sqrt(h2bar))/h2bar^3 );
    df2_dF1 = 0;
    df2_dFd = 0;
    
    Ac = [df1_dh1, df1_dh2;
          df2_dh1, df2_dh2];
    Bc = [df1_dF1; df2_dF1];
    Ec = [df1_dFd; df2_dFd];
    
    Ad = eye(2) + Ts*Ac;
    Bd = Ts*Bc;
    Ed = Ts*Ec;
end