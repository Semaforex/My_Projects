zadanie_3;

% Parametry symulacji
sim_time = 70;
kk = sim_time/Tp;

% Wartość zadana
yzad = zeros(1, kk);
yzad(20:kk) = 1;
% Początkowy zestaw parametrów
% D1 = 40/Tp; % horyzont dynamiki
% N1 = 40/Tp; % horyzont predykcji
% Nu1 = 40/Tp; % horyzont sterowania
% lambda1 = 1;

D1 = 40/Tp; % horyzont dynamiki
N1 = D1;
Nu1 = D1;
lambda1 = 1;


% Zestaw 2 parametrów DMC
D2 = 40/Tp;
N2 = 12/Tp;
Nu2 = 1/Tp;
lambda2 = 3;

% Zestaw 3 parametrów DMC
D3 = 40/Tp;
N3 = 12/Tp;
Nu3 = 1/Tp;
lambda3 = 8;

% Uruchomienie regulatorów DMC z różnymi parametrami
[y_dmc1, u_dmc1] = run_dmc(D1, N1, Nu1, lambda1, kk, b11, b12, a1, a2, yzad);
[y_dmc2, u_dmc2] = run_dmc(D2, N2, Nu2, lambda2, kk, b11, b12, a1, a2, yzad);
[y_dmc3, u_dmc3] = run_dmc(D3, N3, Nu3, lambda3, kk, b11, b12, a1, a2, yzad);

% Wektor czasu
t = 0:Tp:(kk-1)*Tp;

% Wykres odpowiedzi
fig1 = figure; hold on; box on;
stairs(t, yzad, 'k:', 'LineWidth', 1.5);
stairs(t, y_dmc1, 'r', 'LineWidth', 1.2);
stairs(t, y_dmc2, 'b--', 'LineWidth', 1.2);
stairs(t, y_dmc3, 'g--', 'LineWidth', 1.2);
title('Porównanie y(t) DMC dla różnych parametrów lambda (Nu=2, N=24, D=80)');
xlabel('Czas [s]');
ylabel('Wyjscie y(t)');
legend('Wartosc zadana', ...
       ['y(t) dla DMC z lambda' num2str(lambda1)], ...
       ['y(t) dla DMC z lambda' num2str(lambda2)], ...
       ['y(t) dla DMC z lambda' num2str(lambda3)], ...
       'Location', 'southeast');
xlim([0 sim_time]);
grid on;

% Zapis wykresu odpowiedzi
filename = fullfile(output_dir, "DMC_comparison_y.pdf");
exportgraphics(fig1, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

% Wykres sterowania
fig2 = figure; hold on; box on;
stairs(t, u_dmc1, 'r', 'LineWidth', 1.2);
stairs(t, u_dmc2, 'b--', 'LineWidth', 1.2);
stairs(t, u_dmc3, 'g--', 'LineWidth', 1.2);
title('Porównanie u(t) DMC dla różnych parametrów lambda (Nu=2, N=24, D=80)');
xlabel('Czas [s]');
ylabel('Sterowanie u(t)');
legend(['u(t) dla DMC z lambda' num2str(lambda1)], ...
       ['u(t) dla DMC z lambda' num2str(lambda2)], ...
       ['u(t) dla DMC z lambda' num2str(lambda3)], ...
       'Location', 'best');
xlim([0 sim_time]);
grid on;

% Zapis wykresu sterowania
filename = fullfile(output_dir, "DMC_comparison_u.pdf");
exportgraphics(fig2, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

% Komunikat o zakończeniu
disp('Porównanie regulatorów DMC zakończone.');
disp(['Wykresy zapisane w katalogu: ' output_dir]);


function [y_dmc, u_dmc] = run_dmc(D, N, Nu, lambda, kk, b11, b12, a1, a2, yzad)
% Funkcja wykonująca symulację regulatora DMC z podanymi parametrami

% Odpowiedź skokowa obiektu
s = zeros(1, D);
delay = 10;
us = zeros(1, D+delay);
us(delay:D+delay) = 1;
for i = delay+3:D+delay
    s(i-delay) = b11*us(i-11) + b12*us(i-12) - a1*s(i-delay-1) - a2*s(i-delay-2);
end
% Macierz dynamiczna M
M = zeros(N, Nu);
for i = 1:N
    for j = 1:Nu
        if i >= j
            M(i, j) = s(i-j+1);
        end
    end
end

% Macierz dynamiczna Mp (przeszłe sterowania)
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

% Obliczenie macierzy K
K = inv(M'*M + lambda*eye(Nu)) * M';
K1 = K(1, :);

% Inicjalizacja zmiennych dla DMC
u_dmc = zeros(1, kk);
y_dmc = zeros(1, kk);
dUp = zeros(D-1, 1);

% Główna pętla symulacji DMC
for k = 13:kk
    y_dmc(k) = b11*u_dmc(k-11) + b12*u_dmc(k-12) - a1*y_dmc(k-1) - a2*y_dmc(k-2);
    y0 = y_dmc(k) + Mp * dUp;
    du = K1 * (yzad(k)*ones(N,1) - y0);
    u_dmc(k) = u_dmc(k-1) + du;
    dUp = [du; dUp(1:end-1)];
end
end
