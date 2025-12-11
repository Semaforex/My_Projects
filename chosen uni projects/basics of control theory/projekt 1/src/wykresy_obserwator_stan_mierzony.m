% Initialize structure to store results
errors = struct();
macierze;

s_o_vals = [-0.5, -0.7, -1, -3, -10];
T = 15;
s_b = -2;
J = zeros(length(s_o_vals), 1);

% First pass through the loop to find the minimum length of tx
for i = 1:length(s_o_vals)
    disp(['s_o = ', num2str(s_o_vals(i))]);
    s_o = s_o_vals(i);
    [L, K] = parametry_obserwator_regulator(s_b, s_o, A, B, C);
    [L1, L2, L3, L4] = deal(L(1), L(2), L(3), L(4));
    simOut = sim('obiekt_obserwator_regulatorStanMierzony.slx', 'StopTime', num2str(T));
    
    u = simOut.u.signals.values;
    tu = simOut.u.time;
    x = simOut.abs_error_x.signals.values;
    tx = simOut.abs_error_x.time;
    
    
    % Store the current run data in the structure
    errors.x_1_errors{i} = x(:, 1);
    errors.x_2_errors{i} = x(:, 2);
    errors.x_3_errors{i} = x(:, 3);
    errors.x_4_errors{i} = x(:, 4);
    errors.u_errors{i} = u;
    errors.time{i} = tx;  % Store each simulation's time vector

    J(i) = trapz(tx, abs(x(:, 1)) + abs(x(:, 2)) + abs(x(:, 3)) + abs(x(:, 4)));
end

disp('End of simulation loop');

% Plot each dataset with its own time vector
figure;
for i = 1:length(s_o_vals)
    % Plot x1 data using its own time vector
    semilogy(errors.time{i}, errors.x_1_errors{i});
    hold on;
end
title('Bład w x_1 (Skala Logarytmiczna)');
xlabel('Czas');
ylabel('|x_1-xhat_1|');
legend('s_o = ' + string(s_o_vals));
grid on;

figure;
for i = 1:length(s_o_vals)
    % Plot x2 data using its own time vector
    semilogy(errors.time{i}, errors.x_2_errors{i});
    hold on;
end
title('Bład w x_2 (Skala Logarytmiczna)');
xlabel('Czas');
ylabel('|x_2-xhat_2|');
legend('s_o = ' + string(s_o_vals));
grid on;

figure;
for i = 1:length(s_o_vals)
    % Plot x3 data using its own time vector
    semilogy(errors.time{i}, errors.x_3_errors{i});
    hold on;
end
title('Bład w x_3 (Skala Logarytmiczna)');
xlabel('Czas');
ylabel('|x_3-xhat_3|');
legend('s_o = ' + string(s_o_vals));
grid on;

figure;
for i = 1:length(s_o_vals)
    % Plot x4 data using its own time vector
    semilogy(errors.time{i}, errors.x_4_errors{i});
    hold on;
end
title('Bład w x_4 (Skala Logarytmiczna)');
xlabel('Czas');
ylabel('|x_4-xhat_4|');
legend('s_o = ' + string(s_o_vals));
grid on;

for i = 1:length(s_o_vals)
    disp(['Quality Index for s_o = ', num2str(s_o_vals(i)), ': J = ', num2str(J(i))]);
end