%% Skrypt do ręcznego testowania stabilności dla DMC i PID
% Bazowany na zadanie_6_stabilnosc.m

% Wczytanie parametrów
zadanie_3

%% Parametry do ręcznego ustawienia
TGT0s = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0];
kPID = [1.61, 1.53, 1.46, 1.40, 1.34, 1.29, 1.23, 1.19, 1.15, 1.11, 1.07];
kDMC = [2.25, 2.18, 2.09, 1.98, 1.88, 1.78, 1.69, 1.61, 1.53, 1.46, 1.40];

T0 = TGT0s(1);  % Wybór T0
k = kPID(1);  % Wybór k

K_nom = 3.5;  % Nominalna wartość K

K = k * K_nom;  % Wzmocnienie - można zmienić ręcznie

%% Przygotowanie parametrów i macierzy DMC
D = 40/Tp;
N = 12/Tp;
Nu = 1/Tp;
lambda = 3;
sim_time = 500; % Krótszy czas symulacji dla szybszych testów
kk = sim_time/Tp;
delay = 10;

% Obliczenie parametrów modelu dla wybranego wzmocnienia K i T0
G_s = K_nom / ((T1*s+1)*(T2*s+1));
Gz = c2d(G_s, Tp, 'zoh');
[b, a] = tfdata(Gz, 'v');
b1 = b(2);
b2 = b(3);
a1 = a(2);
a2 = a(3);

a_mod = [a1 a2];
b_mod = zeros(1, delay+2);
b_mod(delay+1) = b1;
b_mod(delay+2) = b2;

%% Przygotowanie odpowiedzi skokowej dla DMC
s_test = zeros(1, D);
us_test = zeros(1, D+delay);
us_test(delay:D+delay) = 1;

for j = delay+3:D+delay
    s_test(j-delay) = b1*us_test(j-11) + b2*us_test(j-12) - a1*s_test(j-delay-1) - a2*s_test(j-delay-2);
end

% Tworzenie macierzy M i Mp dla DMC
M_test = zeros(N, Nu);
for ii = 1:N
    for jj = 1:Nu
        if ii >= jj
            M_test(ii, jj) = s_test(ii-jj+1);
        end
    end
end

Mp_test = zeros(N, D-1);
for ii = 1:N
    for jj = 1:D-1
        if ii+jj <= D
            Mp_test(ii, jj) = s_test(ii+jj) - s_test(jj);
        else
            Mp_test(ii, jj) = s_test(D) - s_test(jj);
        end
    end
end

% Parametry regulatora DMC
K_test = inv(M_test'*M_test + lambda*eye(Nu)) * M_test';
K1_test = K_test(1, :);

%% Parametry zmodyfikowanego modelu
G_s = K / ((T1*s+1)*(T2*s+1));
Gz = c2d(G_s, Tp, 'zoh');
[b, a] = tfdata(Gz, 'v');
b1 = b(2);
b2 = b(3);
a1 = a(2);
a2 = a(3);
delay = 10*T0;

a_mod = [a1 a2];
b_mod = zeros(1, delay+2);
b_mod(delay+1) = b1;
b_mod(delay+2) = b2;

%% Symulacja PID
% Parametry regulatora PID
r2 = 1.9875;
r1 = -4.3670;
r0 = 2.3991;

% Inicjalizacja zmiennych dla PID
y_pid = zeros(1, kk);
u_pid = zeros(1, kk);
e_pid = zeros(1, kk);
yzad = zeros(1, kk);
yzad(delay+3:kk) = 1;

% Symulacja PID
for k = delay+3:kk
    y_pid(k) = 0;
    for j = 1:delay+2
        y_pid(k) = y_pid(k) + b_mod(j)*u_pid(k-j);
    end
    for j = 1:2
        y_pid(k) = y_pid(k) - a_mod(j)*y_pid(k-j);
    end
    e_pid(k) = yzad(k) - y_pid(k);
    u_pid(k) = r2*e_pid(k-2) + r1*e_pid(k-1) + r0*e_pid(k) + u_pid(k-1);
end

%% Symulacja DMC
% Inicjalizacja zmiennych dla DMC
y_dmc = zeros(1, kk);
u_dmc = zeros(1, kk);
dUp_test = zeros(D-1, 1);

% Symulacja DMC
for k = delay+3:kk
    y_dmc(k) = 0;
    for j = 1:delay+2
        y_dmc(k) = y_dmc(k) + b_mod(j)*u_dmc(k-j);
    end
    for j = 1:2
        y_dmc(k) = y_dmc(k) - a_mod(j)*y_dmc(k-j);
    end
    y0_test = y_dmc(k) + Mp_test * dUp_test;
    du_test = K1_test * (yzad(k)*ones(N,1) - y0_test);
    u_dmc(k) = u_dmc(k-1) + du_test;
    dUp_test = [du_test; dUp_test(1:end-1)];
end

%% Wykresy
t = 0:Tp:(kk-1)*Tp;

% Wykres odpowiedzi PID
figure;
subplot(2,2,1);
plot(t, yzad, 'r--', 'LineWidth', 1.5);
hold on;
plot(t, y_pid, 'b-', 'LineWidth', 1.5);
title(['Odpowiedź PID dla K = ', num2str(K), ', T0/T0_{nom} = ', num2str(T0)]);
xlabel('Czas [s]');
xlim([0 sim_time]);
ylabel('Wyjście y(t)');
legend('Wartość zadana', 'Wyjście PID');
grid on;

% Wykres sterowania PID
subplot(2,2,2);
plot(t, u_pid, 'g-', 'LineWidth', 1.5);
title('Sygnał sterujący PID');
xlabel('Czas [s]');
xlim([0 sim_time]);
ylabel('Sterowanie u(t)');
grid on;

% Wykres odpowiedzi DMC
subplot(2,2,3);
plot(t, yzad, 'r--', 'LineWidth', 1.5);
hold on;
plot(t, y_dmc, 'b-', 'LineWidth', 1.5);
title(['Odpowiedź DMC dla K = ', num2str(K), ', T0/T0_{nom} = ', num2str(T0)]);
xlabel('Czas [s]');
ylabel('Wyjście y(t)');
xlim([0 sim_time]);
legend('Wartość zadana', 'Wyjście DMC');
grid on;

% Wykres sterowania DMC
subplot(2,2,4);
plot(t, u_dmc, 'g-', 'LineWidth', 1.5);
title('Sygnał sterujący DMC');
xlabel('Czas [s]');
xlim([0 sim_time]);
ylabel('Sterowanie u(t)');
grid on;

gcf = figure; 
T = 1:0.1:2;
plot(T, kPID, 'o-', 'LineWidth', 2, 'DisplayName', 'PID'); hold on;
plot(T, kDMC, 's-', 'LineWidth', 2, 'DisplayName', 'DMC');
legend('granica stabilnosci PID', 'granica stabilnosci DMC', 'Location', 'best');
title('Obszary stabilnosci DMC i PID');
xlabel('T_{gran}/T_0');
ylabel('K_{gran}/K_0');
grid on;

filename = fullfile(output_dir, "stability_areas.pdf");
exportgraphics(gcf, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');