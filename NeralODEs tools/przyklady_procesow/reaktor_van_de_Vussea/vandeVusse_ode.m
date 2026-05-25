function dxdt = vandeVusse_ode(~, x, u)
    % Parametry reaktora Van de Vusse'a
    k1 = 50;     % 1/h
    k2 = 100;     % 1/h
    k3 = 10;     % L/(mol*h)
    CAf = 10;   % mol/L (stężenie na wlocie)
    V = 1;      % l
    
    Ca = x(1);
    Cb = x(2);
    F = u(1); 
    
    dCadt = (F/V) * (CAf - Ca) - k1 * Ca - k3 * Ca^2;
    dCbdt = -(F/V) * Cb + k1 * Ca - k2 * Cb;
    
    dxdt = [dCadt; dCbdt];
end