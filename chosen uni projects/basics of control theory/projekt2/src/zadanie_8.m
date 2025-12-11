%% Skokowa zmiana wartości zadanej


%% GPC zasymulowany w zadanie_7.m
zadanie_7


%% DMC
D = 40/Tp; % horyzont dynamiki
N = 12/Tp; % horyzont predykcji
Nu = 1/Tp; % horyzont sterowania
lambda = 3;

% Wyznaczanie odpowiedzi skokowej modelu dla DMC
s = zeros(1, D);
delay = 10;
us = zeros(1, D+delay);
us(delay:D+delay) = 1;
for i = delay+3:D+delay
    s(i-delay) = b11*us(i-11) + b12*us(i-12) - a1*s(i-delay-1) - a2*s(i-delay-2);
end

% Budowa macierzy M dla DMC
M = zeros(N, Nu);
for i = 1:N
    for j = 1:Nu
        if i >= j
            M(i, j) = s(i-j+1);
        end
    end
end

% Budowa macierzy Mp dla DMC
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

% Wyznaczenie macierzy sterowania K i pierwszego wiersza K1 dla DMC
K = inv(M'*M + lambda*eye(Nu)) * M';
K1 = K(1, :);

% Inicjalizacja zmiennych dla DMC
u_dmc = zeros(1, kk);
y_dmc = zeros(1, kk);
dUp = zeros(D-1, 1);

% Główna pętla obliczeniowa DMC
for k = 13:kk
    y_dmc(k) = b11*u_dmc(k-11) + b12*u_dmc(k-12) - a1*y_dmc(k-1) - a2*y_dmc(k-2);
    y0 = y_dmc(k) + Mp * dUp;
    du = K1 * (yzad(k)*ones(N,1) - y0);
    u_dmc(k) = u_dmc(k-1) + du;
    dUp = [du; dUp(1:end-1)];
end

% Add DMC results to fig1
figure(fig1);
hold on;
stairs(Tp*(1:kk), y_dmc, 'g--', 'LineWidth', 1, 'DisplayName', 'y(t) DMC');
title('wartosc y(t) GPC oraz DMC przy skokowej zmianie zadanej');
figure(fig2);
hold on;
stairs(Tp*(1:kk), u_dmc, 'g--', 'LineWidth', 1, 'DisplayName', 'u(t) DMC');
title('wartosc sygnalu u(t) GPC oraz DMC przy skokowej zmianie zadanej');
legend('show');
hold off;




%% Skokowa wartość niemierzalnego zakłócenia
% Generowanie skokowej wartości niemierzalnego zakłócenia
interference = zeros(1, kk);
interference(20:kk) = 0.1;
yzad = zeros(1, kk);

%% GPC z zakłóceniem

% Inicjalizacja zmiennych dla GPC z zakłóceniem
u = zeros(1, kk);
y = zeros(1, kk);

% Główna pętla obliczeniowa GPC z zakłóceniem
for k = 13:kk
    y(k) = b11*u(k-11) + b12*u(k-12) - a1*y(k-1) - a2*y(k-2) + interference(k);
    d = y(k);
    for i = 1:nb
        d = d - b(i)*u(k-i);
    end
    for i = 1:na
        d = d + a(i)*y(k-i);
    end
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

    du = K1 * (yzad(k)*ones(N,1) - y0');
    u(k) = u(k-1) + du;
end

% Add GPC results to fig3
fig3 = figure;
t = 0:Tp:(kk-1)*Tp;
stairs(t, yzad, 'k:', 'LineWidth', 0.5, 'DisplayName', 'y_{zad}(t) = 0'); hold on;
stairs(t, y, 'b', 'LineWidth', 1, 'DisplayName', 'y(t) GPC z zakloceniem');

fig4 = figure;
stairs(t, u, 'b', 'LineWidth', 1, 'DisplayName', 'u(t) GPC z zakloceniem');

%% DMC z zakłóceniem

% Inicjalizacja zmiennych dla DMC z zakłóceniem
u_dmc = zeros(1, kk);
y_dmc = zeros(1, kk);
dUp = zeros(D-1, 1);

% Główna pętla obliczeniowa DMC z zakłóceniem
for k = 13:kk
    y_dmc(k) = b11*u_dmc(k-11) + b12*u_dmc(k-12) - a1*y_dmc(k-1) - a2*y_dmc(k-2)+ interference(k);
    y0 = y_dmc(k) + Mp * dUp;
    du = K1 * (yzad(k)*ones(N,1) - y0);
    u_dmc(k) = u_dmc(k-1) + du;
    dUp = [du; dUp(1:end-1)];
end


figure(fig3);
hold on;
stairs(t, y_dmc, 'g--', 'LineWidth', 1, 'DisplayName', 'y(t) DMC z zakloceniem');
stairs(t, interference, 'r:', 'LineWidth', 1, 'DisplayName', 'zaklocenie'); hold on;
title('wartosc y(t) GPC oraz DMC z zakloceniem');
xlabel('t [s]');
ylabel('y(t)');
legend('Location', 'best');
hold off;
figure(fig4);
hold on;
stairs(t, u_dmc, 'g--', 'LineWidth', 1, 'DisplayName', 'u(t) DMC z zakloceniem');
title('wartosc sygnalu u(t) GPC oraz DMC z zakloceniem');
xlabel('t [s]');
ylabel('u(t)');
legend('Location', 'best');
hold off;

filename = fullfile(output_dir, "y_skok.pdf");
exportgraphics(fig1, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

filename = fullfile(output_dir, "u_skok.pdf");
exportgraphics(fig2, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

filename = fullfile(output_dir, "y_zakl.pdf");
exportgraphics(fig3, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

filename = fullfile(output_dir, "u_zakl.pdf");
exportgraphics(fig4, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');
