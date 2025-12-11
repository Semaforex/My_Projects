results = struct();
s_b = [-1, -2, -4];
y_zad = 5;
T = 15;
macierze;
B = 1.8*B;

for i = 1:length(s_b)
    s_b_val = s_b(i);
    disp(['s_b = ', num2str(s_b_val)]);
    [K_2, K_I] = parametry_regulator_calkowy(A, B, C, s_b_val); %B/1.8
    simOut = sim('regulator_calkowanie.slx', 'StopTime', num2str(T));

    u = simOut.u.signals.values;
    tu = simOut.u.time;
    x = simOut.x.signals.values;
    tx = simOut.x.time;
    y = simOut.y.signals.values;
    ty = simOut.y.time;

    results.u{i} = u;
    results.tu{i} = tu;
    results.x{i} = x;
    results.tx{i} = tx;
    results.y{i} = y;
    results.ty{i} = ty;
end

disp('End of simulation loop');

figure;
for i = 1:length(s_b)
    plot(results.tx{i}, results.x{i}(:, 1));
    hold on;
end
title('Stan x_1');
xlabel('Czas');
ylabel('x_1');
legend('s_b = ' + string(s_b));

figure;
for i = 1:length(s_b)
    plot(results.tx{i}, results.x{i}(:, 2));
    hold on;
end
title('Stan x_2');
xlabel('Czas');
ylabel('x_2');
legend('s_b = ' + string(s_b));

figure;
for i = 1:length(s_b)
    plot(results.tx{i}, results.x{i}(:, 3));
    hold on;
end
title('Stan x_3');
xlabel('Czas');
ylabel('x_3');
legend('s_b = ' + string(s_b));

figure;
for i = 1:length(s_b)
    plot(results.tx{i}, results.x{i}(:, 4));
    hold on;
end
title('Stan x_4');
xlabel('Czas');
ylabel('x_4');
legend('s_b = ' + string(s_b));

figure;
plot(results.ty{i}, y_zad * ones(size(results.ty{i})), 'r--');
hold on;
for i = 1:length(s_b)
    plot(results.ty{i}, results.y{i});
    hold on;
    title('Wyjście y oraz wartość zadana');
    xlabel('Czas');
    ylabel('y');
end
legend(['Wartość zadana', 's_b = ' + string(s_b)]);

figure;
for i = 1:length(s_b)
    plot(results.tu{i}, results.u{i});
    hold on;
end
title('Sygnał sterujący u');
xlabel('Czas');
ylabel('u');
legend('s_b = ' + string(s_b));

