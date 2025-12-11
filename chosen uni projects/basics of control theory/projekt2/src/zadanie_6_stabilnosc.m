%% Parametry cyfrowego PID oraz oryginalnego procesu z poprzednich zadań
zadanie_3


%% Przygotowanie parametrów i macierzy DMC (parametry z zadania 5)
D = 40/Tp;
N = 12/Tp;
Nu = 1/Tp;
lambda = 3;

s_test = zeros(1, D);
delay = 10;
us_test = zeros(1, D+delay);
us_test(delay:D+delay) = 1;
for j = delay+3:D+delay
    s_test(j-delay) = b11*us_test(j-11) + b12*us_test(j-12) - a1*s_test(j-delay-1) - a2*s_test(j-delay-2);
end

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

K_test = inv(M_test'*M_test + lambda*eye(Nu)) * M_test';
K1_test = K_test(1, :);



%% Obszary stabilnosci algorytmow DMC i PID
K_nom = 3.5;
sim_time = 1000;
kk=sim_time/Tp;
delay_values = 10:1:20;
max_K = 20;
step_K = 0.01;
K_stability_PID = zeros(size(delay_values));
K_stability_DMC = zeros(size(delay_values));

for i = 1:length(delay_values)
    delay = delay_values(i);
    % Znajdowanie granic stabilności dla PID
    for K = K_nom:step_K:max_K
        % Obliczenie parametrów modelu dla zmodyfikowanych wartości
        G_s = K / ((T1*s+1)*(T2*s+1));
        Gz = c2d(G_s, Tp, 'zoh');
        [bt, at] = tfdata(Gz, 'v');
        b1 = b(2);
        b2 = b(3);
        a = [at(2) at(3)];
        b = zeros(1, delay+2);
        b(delay+1) = bt(2); b(delay+2) = bt(3);
        is_stable_PID = true;
        y_test = zeros(1, kk);
        u_test = zeros(1, kk);
        e_test = zeros(1, kk);
        yzad_test = zeros(1, kk);
        yzad_test(delay+3:kk) = 1;
        
        for k=delay+3:kk
            y_test(k) = 0;
            for j=1:delay+2
                y_test(k) = y_test(k) + b(j)*u_test(k-j);
            end
            for j=1:2
                y_test(k) = y_test(k) - a(j)*y_test(k-j);
            end
            e_test(k) = yzad_test(k) - y_test(k);
            u_test(k) = r2*e_test(k-2) + r1*e_test(k-1) + r0*e_test(k) + u_test(k-1);
        end
        if ~check_stability(y_test, yzad_test(kk))
            K_stability_PID(i) = K/K_nom;
            break;
        end
    end
    
    % Znajdowanie granic stabilności dla DMC
    for K = K_nom:step_K:max_K
        % Obliczenie parametrów modelu dla zmodyfikowanych wartości
        G_s = K / ((T1*s+1)*(T2*s+1));
        Gz = c2d(G_s, Tp, 'zoh');
        [bt, at] = tfdata(Gz, 'v');
        b1 = b(2);
        b2 = b(3);
        a = [at(2) at(3)];
        b = zeros(1, delay+2);
        b(delay+1) = bt(2); b(delay+2) = bt(3);
        is_stable_DMC = true;

        y_test = zeros(1, kk);
        u_test = zeros(1, kk);
        dUp_test = zeros(D-1, 1);
        yzad_test = zeros(1, kk);
        yzad_test(delay+3:kk) = 1;
        
        for k = delay+3:kk
            y_test(k) = 0;
            for j=1:delay+2
                y_test(k) = y_test(k) + b(j)*u_test(k-j);
            end
            for j=1:2
                y_test(k) = y_test(k) - a(j)*y_test(k-j);
            end
            y0_test = y_test(k) + Mp_test * dUp_test;
            du_test = K1_test * (yzad_test(k)*ones(N,1) - y0_test);
            u_test(k) = u_test(k-1) + du_test;
            dUp_test = [du_test; dUp_test(1:end-1)];
        end
        if ~check_stability(y_test, yzad_test(kk))
            K_stability_DMC(i) = K/K_nom;
            break;
        end
    end
end

% Wykreślenie krzywych stabilności


disp('Wyniki analizy stabilności:');
results_table = table(delay_values', K_stability_PID', K_stability_DMC', ...
    'VariableNames', {'T0_gran:Tp', 'k_PID', 'k_DMC'});
disp(results_table);


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




