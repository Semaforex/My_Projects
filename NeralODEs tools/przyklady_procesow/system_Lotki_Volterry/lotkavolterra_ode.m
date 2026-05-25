function xdot = lotkavolterra_ode(~,x,u)
alv = 0.5; blv = 0.025; clv = 0.5; dlv = 0.005;

xdot = [alv*x(1) - blv*x(1)*x(2);
    -clv*x(2) + dlv*x(1)*x(2) + u];