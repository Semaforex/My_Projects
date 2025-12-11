clear;
output_dir = '/home/mparadow/Desktop/stp/projekt2/wykresy';



zadanie_2
na = 2;
nb = 12;
D = 40/Tp; 
N = 12/Tp; 
Nu = 1/Tp; 
lambda = 3;
Tp = 0.5;
sim_time = 60;
kk = sim_time/Tp;
% Wektory współczynników modelu
a = [a1 a2];
b = [0 0 0 0 0 0 0 0 0 0 b11 b12];

% Wyznaczanie odpowiedzi skokowej modelu
s = zeros(1, N);
for j = 1:N
    for i = 1:min(j, nb)
        s(j) = s(j) + b(i);
    end
    for i = 1:min(j-1, na)
        s(j) = s(j) - a(i)*s(j-i);
    end
end

% Budowa macierzy M dla algorytmu GPC
M = zeros(N, Nu);
for i = 1:N
    for j = 1:Nu
        if i >= j
            M(i, j) = s(i-j+1);
        end
    end
end
% Wyznaczenie macierzy sterowania K i pierwszego wiersza K1
K = inv(M'*M + lambda*eye(Nu)) * M';
K1 = K(1, :);

% Inicjalizacja zmiennych sterowania i zadanej trajektorii
u = zeros(1, kk);
yzad = zeros(1, kk);
yzad(20:kk) = 1;
y = zeros(1, kk);

% Główna pętla obliczeniowa GPC
for k = 13:kk
    y(k) = b11*u(k-11) + b12*u(k-12) - a1*y(k-1) - a2*y(k-2);
    % Obliczanie zakłócenia modelu
    d = y(k);
    for i = 1:nb
        d = d - b(i)*u(k-i);
    end
    for i = 1:na
        d = d + a(i)*y(k-i);
    end
    % Predykcja wyjścia na horyzoncie predykcji
    y0 = zeros(1, N);
    for p = 1:N
        for i = 1:min(p, nb)
            y0(p) = y0(p) + b(i)*u(k-1);
        end
        for i = min(p, nb)+1:nb
            y0(p) = y0(p) + b(i)*u(k-i+p);
        end
        for i = 1:min(p-1, na)
            y0(p) = y0(p) - a(i)*y0(p-i);
        end
        for i = min(p-1, na)+1:na
            y0(p) = y0(p) - a(i)*y(k-i+p);
        end
        y0(p) = y0(p) + d;
    end

    % Wyznaczenie przyrostu sterowania
    du = K1 * (yzad(k)*ones(N,1) - y0');
    u(k) = u(k-1) + du;
end

fig1 = figure;
t = 0:Tp:(kk-1)*Tp;
stairs(t, yzad, 'k:', 'LineWidth', 0.5, "DisplayName", "y_{zad}(t)"); hold on;
stairs(t, y, 'b', 'LineWidth', 1.5, "DisplayName", 'y(t) GPC');
xlabel('Czas [s]');
ylabel('y(t)');
legend('show');
title('wyjscie modelu regulowanego GPC');

filename = fullfile(output_dir, "GPC_y.pdf");
exportgraphics(fig1, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

fig2 = figure;
stairs(t, u, 'b', 'LineWidth', 1.5, "DisplayName", 'u(t) GPC');
xlabel('Czas [s]');
ylabel('u(t)');
legend('show');
title('sygnal sterujacy GPC');
filename = fullfile(output_dir, "GPC_u.pdf");
exportgraphics(fig2, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');
