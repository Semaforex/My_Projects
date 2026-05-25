%% okres próbkowania modelu w regulacji dt=0,01 lub mniej
%% zakres sterowania u=10...80
%% warunek początkowy u0=40, x0=[1.282; 0.814]

%% w regulacji: wejście u, wyjście y=x1

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);

dt = 0.01; % h za dużo (,,kanciasty'' wykres fazowy, trajektorie za szybkie)
%dt = 0.005; % h
%dt = 0.0025; % h
%dt = 0.001; % h

switch dt
    case 0.01
        u_okres_zmian = 5;     
    case 0.005
        u_okres_zmian = 10; 
    case 0.0025
        u_okres_zmian = 20;         
    case 0.001
        u_okres_zmian = 50;                 
end
u_liczba_okresow = 5;
u_rozbieg = 10;      
u_min = 10; u_max = 80;

% Warunki początkowe
u0 = 40;
x0 = [1.282000; 0.814000];

u_dyskr = u_min + (u_max - u_min) * rand(1, u_liczba_okresow);
u_dyskr = kron(u_dyskr, ones(1, u_okres_zmian));
u_dyskr = [u0 * ones(1, u_rozbieg) u_dyskr];

kmax = length(u_dyskr);
Ca_dyskr = zeros(1, kmax); 
Cb_dyskr = zeros(1, kmax); 

for k = 1:kmax
    tpsym = (k - 1) * dt;
    tksym = k * dt;
    
    u = u_dyskr(k);
    
    [~, x_ode] = ode45(@(t, x) vandeVusse_ode(t, x, u), ...
        [tpsym tksym], x0);
    
    Ca_dyskr(k) = x_ode(end, 1);
    Cb_dyskr(k) = x_ode(end, 2);

    x0 = x_ode(end, :)';
end

figure;
subplot(2, 1, 1);
stairs(u_dyskr);
xlabel('k'); ylabel('u (przepływ F)');
grid on;

subplot(2, 1, 2);
plot(Ca_dyskr);
hold on;
plot(Cb_dyskr);
xlabel('k'); ylabel('Stężenie [mol/L]');
legend('Ca', 'Cb');
grid on;

figure;
plot(Ca_dyskr, Cb_dyskr);
xlabel('Ca [mol/L]');
ylabel('Cb [mol/L]');
grid on;

disp(sprintf('u (F): min=%8.3f, max=%8.3f, zakres=%8.3f', min(u_dyskr), max(u_dyskr), max(u_dyskr) - min(u_dyskr)));
disp(sprintf('Ca:    min=%8.3f, max=%8.3f, zakres=%8.3f', min(Ca_dyskr), max(Ca_dyskr), max(Ca_dyskr) - min(Ca_dyskr)));
disp(sprintf('Cb:    min=%8.3f, max=%8.3f, zakres=%8.3f', min(Cb_dyskr), max(Cb_dyskr), max(Cb_dyskr) - min(Cb_dyskr)));


