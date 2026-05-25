%% u=FI, y=NAMW
%% umin = 0.003; umax = 0.06 zgodnie z pracą Doyle'a, nie zmieniać
%% dt = 0.03
%% u0=0.016783, x0=[5.50678139602962258000e+00; 1.32905703708103201000e-01; 1.97532716825933033000e-03; 4.93816858382551445000e+01]

%% w regulacji: wejście u=FI, wyjście y=NAMW

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);
kolory = lines;

dt = 0.03;

u_okres_zmian = 40;
u_liczba_okresow = 10;
u_rozbieg = 10;      
u_min = 0.003; u_max = 0.06;

% warunki początkowe
u0 = 0.016783;
x0 = [5.50678139602962258000e+00;
      1.32905703708103201000e-01;
      1.97532716825933033000e-03;
      4.93816858382551445000e+01];

u_dyskr = u_min + (u_max - u_min) * rand(1, u_liczba_okresow);
u_dyskr = kron(u_dyskr, ones(1, u_okres_zmian));
u_dyskr = [u0 * ones(1, u_rozbieg) u_dyskr];

kmax = length(u_dyskr);
x_all_dyskr = zeros(4, kmax);
y_dyskr = zeros(1, kmax);    

for k = 1:kmax
    tpsym = (k - 1) * dt;
    tksym = k * dt;
    u = u_dyskr(k);
    
    [~, x_ode] = ode45(@(t, x) polymerization_ode(t, x, u), ...
        [tpsym tksym], x0);
    
    x_final_step = x_ode(end, :)';
    
    x_all_dyskr(:, k) = x_final_step;
    
    y_dyskr(k) = polymerization_outputs([], x_final_step, u);

    x0 = x_final_step;
end

figure;
subplot(2, 1, 1);
stairs(u_dyskr);
xlabel('k'); ylabel('u');
grid on;

subplot(2, 1, 2);
plot(y_dyskr);
xlabel('k'); ylabel('y');
grid on;

figure;
tiledlayout(2, 2, 'TileSpacing', 'compact', 'Padding', 'compact');
for i = 1:4
    nexttile;    
    plot(x_all_dyskr(i, :),'Color',kolory(i, :));    
    xlabel('k');
    ylabel(sprintf('x_{%d}', i));
    grid on;
end

