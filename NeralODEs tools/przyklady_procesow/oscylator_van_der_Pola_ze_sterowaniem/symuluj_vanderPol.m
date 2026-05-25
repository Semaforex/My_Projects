%% zakres sterowania u=-1...1
%% zakres zmiennych stanu x=[-1,1] x [-1,1]

%% w regulacji: wejście u, wyjście y=x1

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);

dt = 0.01;%oryginalna praca Kordy i Mezica
%dt = 0.05;%jeszcze działa
%dt = 0.1;%za dużo, wykresy kanciaste

u_okres_zmian = 250;    
u_liczba_okresow = 10; 
u_rozbieg = 50;        
u_min = -1; u_max = 1; 
u0 = 0;                

% znajdowanie punkt na cyklu granicznym (brak stanu ustalonego)
x_start_random = -1 + 2*rand(2,1);

T_pre_sim = 100;
[t_pre, x_pre] = ode45(@(t, x) vanderPol_ode(t, x, 0), [0 T_pre_sim], x_start_random);
x0 = x_pre(end, :)';
disp(sprintf('Wyznaczono warunek początkowy: x0 = [%.4f; %.4f]', x0(1), x0(2)));

figure;
plot(x_pre(:,1), x_pre(:,2));
hold on;
plot(x_start_random(1), x_start_random(2), 'ro', 'MarkerSize', 10, 'MarkerFaceColor','r');
plot(x0(1), x0(2), 'go', 'MarkerSize', 10, 'MarkerFaceColor','g');
title('Wyznaczanie warunku początkowego');
xlabel('x_1'); ylabel('x_2');
legend('Trajektoria do cyklu granicznego', 'Losowy start', 'Wybrany punkt x0');
grid on;

%właściwa symulacja
u_dyskr = u_min + (u_max - u_min) * rand(1, u_liczba_okresow);
u_dyskr = kron(u_dyskr, ones(1, u_okres_zmian));
u_dyskr = [u0 * ones(1, u_rozbieg) u_dyskr];

kmax = length(u_dyskr);
x1_dyskr = zeros(1, kmax);
x2_dyskr = zeros(1, kmax);

for k = 1:kmax
    tpsym = (k - 1) * dt;
    tksym = k * dt;
    u = u_dyskr(k);
    
    [~, x_ode] = ode45(@(t, x) vanderPol_ode(t, x, u), [tpsym tksym], x0);
    
    x1_dyskr(k) = x_ode(end, 1);
    x2_dyskr(k) = x_ode(end, 2);

    x0 = x_ode(end, :)';
end

figure;
subplot(2, 1, 1);
stairs(u_dyskr);
xlabel('k'); ylabel('u');
grid on;

subplot(2, 1, 2);
plot(x1_dyskr);
hold on;
plot(x2_dyskr);
xlabel('k'); ylabel('x_1, x_2');
legend('x_1', 'x_2');
grid on;

figure;
plot(x1_dyskr, x2_dyskr);
hold on;
plot(x1_dyskr(1), x2_dyskr(1));
plot(x1_dyskr(end), x2_dyskr(end));
xlabel('x_1');
ylabel('x_2');
grid on;
