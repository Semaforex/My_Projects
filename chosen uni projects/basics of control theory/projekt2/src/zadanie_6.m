zadanie_2
sim_time = 70;
kk=sim_time/Tp;

%% PID

r2=1.9875;
r1=-4.3670;
r0=2.3991;

y = zeros(1, kk);
u = zeros(1, kk);
yzad = zeros(1, kk);
e = zeros(1, kk);
yzad(10:kk) = 1;
for k=13:kk
    y(k)=b11*u(k-11)+b12*u(k-12)-a1*y(k-1)-a2*y(k-2);
    e(k)=yzad(k)-y(k);
    u(k)=r2*e(k-2)+r1*e(k-1)+r0*e(k)+u(k-1);
end

%% DMC
D = 40/Tp; % horyzont dynamiki
N = 12/Tp; % horyzont predykcji
Nu = 1/Tp; % horyzont sterowania
lambda = 3;

s = zeros(1, D);
delay = 10;
us = zeros(1, D+delay);
us(delay:D+delay) = 1;
for i = delay+3:D+delay
    s(i-delay) = b11*us(i-11) + b12*us(i-12) - a1*s(i-delay-1) - a2*s(i-delay-2);
end

M = zeros(N, Nu);
for i = 1:N
    for j = 1:Nu
        if i >= j
            M(i, j) = s(i-j+1);
        end
    end
end

Mp = zeros(N, D-1);
for i = 1:N
    for j = 1:D-1
        if i+j <= D
            Mp(i, j) = s(i+j) - s(j);
        else
            Mp(i, j) = s(D) - s(j);
        end
    end
end

K = inv(M'*M + lambda*eye(Nu)) * M';
K1 = K(1, :);

u_dmc = zeros(1, kk);
y_dmc = zeros(1, kk);
dUp = zeros(D-1, 1);
yzad = zeros(1, kk);
yzad(20:kk)=1;

for k = 13:kk
    y_dmc(k) = b11*u_dmc(k-11) + b12*u_dmc(k-12) - a1*y_dmc(k-1) - a2*y_dmc(k-2);
    y0 = y_dmc(k) + Mp * dUp;
    du = K1 * (yzad(k)*ones(N,1) - y0);
    u_dmc(k) = u_dmc(k-1) + du;
    dUp = [du; dUp(1:end-1)];
end


%% Wykresy
t = 0:Tp:(kk-1)*Tp; % wektor czasu dopasowany do okresu próbkowania
size(t)
size(y)
size(yzad)
size(y_dmc)
size(u)
size(u_dmc)

fig = figure; hold on; box on;
stairs(t, yzad,':'); stairs(t, y_dmc); stairs(t, y, '--'); 

title('Wykresy odpowiedzi skokowych przy regulacji DMC oraz PID');
xlabel('t [s]');
ylabel('y(t)');
legend('y zad', 'y DMC', 'y PID')
xlim([0 sim_time]); grid on;
filename = fullfile(output_dir, "DMCPID_y.pdf");
exportgraphics(fig, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

fig = figure; hold on; box on;
stairs(t, u_dmc); stairs(t, u, '--'); 
title('Wykresy sygnałów sterujacych przy regulacji DMC oraz PID');
xlabel('t [s]');
ylabel('u(t)');
legend('u DMC', 'u PID');
xlim([0 sim_time]); grid on;
filename = fullfile(output_dir, "DMCPID_u.pdf");
exportgraphics(fig, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');


