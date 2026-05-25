function dxdt = vanderPol_ode(~, x, u)
    dx1dt= 2*x(2);
    dx2dt= -0.8*x(1) - 10*x(1).^2.*x(2) + 2*x(2) + u;

    dxdt = [dx1dt; dx2dt];
end
