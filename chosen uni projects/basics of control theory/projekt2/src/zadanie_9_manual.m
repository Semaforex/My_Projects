zadanie_3




TGT0s = [1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0];
kGPCs = [4.645, 4.540, 4.42, 4.30, 4.19, 4.08, 3.98, 3.878, 3.795, 3.71, 3.632];

T0 = TGT0s(1);  % Wybór T0
k = kGPCs(1);  % Wybór k

K_nom = 3.5;  % Nominalna wartość K

K_test = k * K_nom;  % Wzmocnienie - można zmienić ręcznie

%% Przygotowanie parametrów i macierzy DMC
D = 40/Tp;
N = 12/Tp;
Nu = 1/Tp;
lambda = 3;
Tp = 0.5;
sim_time = 500;
kk = sim_time/Tp;
a = [a1 a2];
b = [0 0 0 0 0 0 0 0 0 0 b11 b12];
nb = length(b);
na = length(a);
a_original = a; % Zachowanie oryginalnych parametrów a
b_original = b; % Zachowanie oryginalnych parametrów b
na_original = na; % Zachowanie oryginalnej długości a
nb_original = nb; % Zachowanie oryginalnej długości b
delay_values = 10:1:20; % Wartości opóźnienia do testowania
K_nom = 3.5; % Nominalna wartość K
max_K = 200; % Maksymalna wartość K
step_K = 0.01; % Krok zmiany K
K_stability_GPC = zeros(1, length(delay_values));

s_test = zeros(1, N);
for j = 1:N
    for i = 1:min(j, nb)
        s_test(j) = s_test(j) + b(i);
    end
    for i = 1:min(j-1, na)
        s_test(j) = s_test(j) - a(i)*s_test(j-i);
    end
end

M = zeros(N, Nu);
for i = 1:N
    for j = 1:Nu
        if i >= j
            M(i, j) = s_test(i-j+1);
        end
    end
end
K = inv(M'*M + lambda*eye(Nu)) * M';
K1 = K(1, :);

delay = 10*T0;
G_s = K_test / ((T1*s+1)*(T2*s+1));
Gz = c2d(G_s, Tp, 'zoh');
[b, a] = tfdata(Gz, 'v');
b1 = b(2);
b2 = b(3);
a1 = a(2);
a2 = a(3);
delay = 10*T0;

a = [a1 a2];
b = zeros(1, delay+2);
na = length(a);
nb = length(b);
b(delay+1) = b1;
b(delay+2) = b2;


u = zeros(1, kk);
yzad = zeros(1, kk);
yzad(delay+3:kk) = 1;
y = zeros(1, kk);

for k = delay+3:kk
    y(k) = 0;
    for j = 1:delay+2
        y(k) = y(k) + b(j)*u(k-j);
    end
    for j = 1:2
        y(k) = y(k) - a(j)*y(k-j);
    end
    y0 = zeros(1, N);
    for p = 1:N
        for j = 1:min(p, nb_original)
            y0(p) = y0(p) + b_original(j)*u(k-1);
        end
        for j = min(p, nb_original)+1:nb_original
            y0(p) = y0(p) + b_original(j)*u(k-j+p);
        end
        for j = 1:min(p-1, na_original)
            y0(p) = y0(p) - a_original(j)*y0(p-j);
        end
        for j = min(p-1, na_original)+1:na_original
            y0(p) = y0(p) - a_original(j)*y(k-j+p);
        end
        y0(p) = y0(p);
    end
    du = K1 * (yzad(k)*ones(N,1) - y0');
    u(k) = u(k-1) + du;
end


% Wykreślenie wyników
figure;
t = 0:Tp:(kk-1)*Tp;
plot(t, y, 'b', 'DisplayName', 'Odpowiedź systemu');
hold on;
plot(t, yzad, 'r--', 'DisplayName', 'Zadana wartość');
xlabel('Czas (s)');
ylabel('Amplituda');
title('Odpowiedź systemu GPC');
legend('show');
grid on;
% Wykres sygnału sterującego
figure;
plot(t, u, 'g', 'DisplayName', 'Sygnał sterujący');
xlabel('Czas (s)');
ylabel('Amplituda');
title('Sygnał sterujący GPC');
legend('show');
% Sprawdzenie stabilności

figure = gcf;
kPID = [1.61, 1.53, 1.46, 1.40, 1.34, 1.29, 1.23, 1.19, 1.15, 1.11, 1.07];
kDMC = [2.25, 2.18, 2.09, 1.98, 1.88, 1.78, 1.69, 1.61, 1.53, 1.46, 1.40];
plot(TGT0s, kPID, 'o-', 'LineWidth', 2, 'DisplayName', 'PID'); hold on;
plot(TGT0s, kDMC, 's-', 'LineWidth', 2, 'DisplayName', 'DMC');
plot(TGT0s, kGPCs, 'o-', 'LineWidth', 2, 'DisplayName', 'GPC');
legend('granica stabilnosci PID', 'granica stabilnosci DMC', 'granica stabilności GPC', 'Location', 'southwest');
title('Obszary stabilnosci GPC, DMC i cyfrowego PID')
xlabel('T_0/(T_0)_{nom}');
ylabel('K/K_{nom}');
ylim([0 5]);
grid on;   

filename = fullfile(output_dir, "stability_area_GPC.pdf");
exportgraphics(gcf, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

disp('Wyniki analizy stabilnosci:');
results_table = table(10*TGT0s', kGPCs', ...
    'VariableNames', {'T_G:Tp', 'K_stability_GPC'});
disp(results_table);

