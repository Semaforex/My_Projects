%% okres próbkowania modelu w regulacji dt=0.1
%% zakres sterowania u=0...10
%% warunek początkowy u0=5, x0=[50; 20]

%% w regulacji: wejście u, wyjście y=x1

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);

dt=0.1;

u_okres_zmian=100;  
u_liczba_okresow=20;
u_rozbieg=10;       
u_min=0; u_max=10;   

% Warunki początkowe
x0=[50; 20];
alv = 0.5; blv = 0.025; clv = 0.5; dlv = 0.005;
u0=clv*x0(2)-dlv*x0(1)*x0(2);%u0=5 dla x0=[50; 20];

u_dyskr=u_min+(u_max-u_min)*rand(1,u_liczba_okresow);
u_dyskr=kron(u_dyskr, ones(1,u_okres_zmian));
%rozbiegówka
u_dyskr=[u0*ones(1,u_rozbieg) u_dyskr];

kmax=length(u_dyskr);
x1_dyskr = zeros(1,kmax);
x2_dyskr = zeros(1,kmax);

for k=1:kmax
    tpsym = (k-1)*dt;
    tksym = k*dt;

    %aktualne sterowanie
    u = u_dyskr(k);
       
    [~, x_ode] = ode45(@(t,x) lotkavolterra_ode(t,x,u), ...
        [tpsym tksym], x0);
    
    x1_dyskr(k)=x_ode(end,1);
    x2_dyskr(k)=x_ode(end,2);

    x0 = x_ode(end,:)';
end

figure;
subplot(2,1,1);
stairs(u_dyskr);
xlabel('k'); ylabel('u');
grid on;

subplot(2,1,2);
plot(x1_dyskr);
hold on;
plot(x2_dyskr);
xlabel('k'); ylabel('x1, x2');
legend('x1 (ofiary)', 'x2 (drapieżniki)');
grid on;

figure;
plot(x1_dyskr, x2_dyskr);
xlabel('x1'); ylabel('x2');


disp(sprintf('u:  min=%8.3f, max=%8.3f, zakres=%8.3f',min(u_dyskr),max(u_dyskr),max(u_dyskr)-min(u_dyskr)));
disp(sprintf('x1: min=%8.3f, max=%8.3f, zakres=%8.3f',min(x1_dyskr),max(x1_dyskr),max(x1_dyskr)-min(x1_dyskr)));
disp(sprintf('x2: min=%8.3f, max=%8.3f, zakres=%8.3f',min(x2_dyskr),max(x2_dyskr),max(x2_dyskr)-min(x2_dyskr)));


