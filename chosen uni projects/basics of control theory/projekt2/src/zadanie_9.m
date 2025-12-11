%% Parametry cyfrowego GCP oraz oryginalnego procesu z poprzednich zadań
zadanie_3


%% Przygotowanie parametrów i macierzy GCP (parametry z zadania 5)
D = 40/Tp; % horyzont dynamiki
N = 12/Tp; % horyzont predykcji
Nu = 1/Tp; % horyzont sterowania
lambda = 3;
Tp = 0.5;
sim_time = 1500;
kk = sim_time/Tp;
% Przygotowanie parametrów modelu i zachowanie oryginalnych wartości
a = [a1 a2];
b = [0 0 0 0 0 0 0 0 0 0 b11 b12];
a_original = a; % Zachowanie oryginalnych parametrów a
b_original = b; % Zachowanie oryginalnych parametrów b
nb = length(b);
na = length(a);
na_original = na; % Zachowanie oryginalnej długości a
nb_original = nb; % Zachowanie oryginalnej długości b

% Wyznaczanie odpowiedzi skokowej modelu
s_test = zeros(1, N);
for j = 1:N
    for i = 1:min(j, nb)
        s_test(j) = s_test(j) + b(i);
    end
    for i = 1:min(j-1, na)
        s_test(j) = s_test(j) - a(i)*s_test(j-i);
    end
end

% Budowa macierzy M i wyznaczenie macierzy sterowania K
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

delay_values = 10:1:20; % Wartości opóźnienia do testowania
K_nom = 3.5; % Nominalna wartość K
max_K = 200; % Maksymalna wartość K
step_K = 0.01; % Krok zmiany K
K_stability_GPC = zeros(1, length(delay_values));

% Pętla po różnych wartościach opóźnienia
for i = 1:length(delay_values)
    delay = delay_values(i);
    is_stable_GPC = true; % Flaga stabilności dla GCP
    
    % Znajdowanie granic stabilności dla GCP
    for K = K_nom:step_K:max_K
        % Obliczenie parametrów modelu dla zmodyfikowanych wartości
        G_s = K / ((T1*s+1)*(T2*s+1));
        Gz = c2d(G_s, Tp, 'zoh');
        [b, a] = tfdata(Gz, 'v');
        b1 = b(2);
        b2 = b(3);
        a1 = a(2);
        a2 = a(3);
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

        % Główna pętla obliczeniowa dla danego K i opóźnienia
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
        % Sprawdzenie stabilności odpowiedzi
        if ~check_stability(y, yzad(kk))
            is_stable_GPC = false;
            disp('unstable')
            disp(K)
        else
            is_stable_GPC = true;
        end
        if ~is_stable_GPC
            K_stability_GPC(i) = K/K_nom;
            break;
        end
        if K == max_K
            K_stability_GPC(i) = K/K_nom;
        end
    end
end
disp('Wyniki analizy stabilności:');
results_table = table(delay_values', K_stability_GPC', ...
    'VariableNames', {'TG:Tp', 'k_GPC'});
disp(results_table);


% Funkcja sprawdzająca stabilność odpowiedzi
function stable = check_stability(y, yzad)
    y = y(0.7* length(y):end);
    tol = 0.1;
    stable = true;
    for i = 2:length(y)
        cond1 = abs(y(i) - yzad) > abs(y(i-1) - yzad)+tol;
        cond2 = abs(y(i) - yzad) > tol;
        if cond1 && cond2
            stable = false; 
        end
    end
end
