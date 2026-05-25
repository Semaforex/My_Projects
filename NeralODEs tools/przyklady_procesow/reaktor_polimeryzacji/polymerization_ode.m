function dxdt = polymerization_ode(t, x, u)
dxdt = zeros(4,1);
dxdt(1) = 60-10*x(1)-2.4568*x(1)*sqrt(x(2));
dxdt(2) = 80*u-10.1022*x(2);
dxdt(3) = 0.0024121*x(1)*sqrt(x(2))+0.112191*x(2)-10*x(3);
dxdt(4) = 245.978*x(1)*sqrt(x(2))-10*x(4);
end