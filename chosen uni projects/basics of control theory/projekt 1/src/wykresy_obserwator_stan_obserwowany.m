% Initialize structure to store results
errors = struct();
macierze;
s_o = -3;
T = 15;
s_b = -2;
min_length = Inf; % Initialize to a large value

% First pass through the loop to find the minimum length of tx
[L, K] = parametry_obserwator_regulator(s_b, s_o, A, B, C);
[L1, L2, L3, L4] = deal(L(1), L(2), L(3), L(4));

obs1 = 0;
obs2 = 0;
obs3 = 0;
obs4 = 0;

simOut = sim('obiekt_obserwator_regulatorStanObserwowany.slx', 'StopTime', num2str(T));

u1 = simOut.u.signals.values;
tu1 = simOut.u.time;
x1 = simOut.x.signals.values;
tx1 = simOut.x.time;
xhat1 = simOut.xhat.signals.values;
txhat1 = simOut.xhat.time;

obs1 = -10;
obs2 = 10;
obs3 = -20;
obs4 = 20;

simOut = sim('obiekt_obserwator_regulatorStanObserwowany.slx', 'StopTime', num2str(T));

u2 = simOut.u.signals.values;
tu2 = simOut.u.time;
x2 = simOut.x.signals.values;
tx2 = simOut.x.time;
xhat2 = simOut.xhat.signals.values;
txhat2 = simOut.xhat.time;


figure;
plot(tu1, x1(:, 1));
hold on;
plot(tu2, x2(:, 1));
% hold on;
% plot(tu1, xhat1(:, 1));
% hold on;
% plot(tu2, xhat2(:, 1));
legend('Stan mierzony przy zerowym stanie pocz. obs.', 'Stan mierzony przy niezerowym stanie pocz. obs.');%, 'Stan obserwowany przy zerowym stanie pocz. obs.', 'Stan obserwowany przy niezerowym stanie pocz. obs.');
hold on;
title('Wykres stanu x_1');
xlabel('Czas');
ylabel('x_1');

figure;
plot(tu1, x1(:, 2));
hold on;
plot(tu2, x2(:, 2));
% hold on;
% plot(tu1, xhat1(:, 2));
% hold on;
% plot(tu2, xhat2(:, 2));
legend('Stan mierzony przy zerowym stanie pocz. obs.', 'Stan mierzony przy niezerowym stanie pocz. obs.');%, 'Stan obserwowany przy zerowym stanie pocz. obs.', 'Stan obserwowany przy niezerowym stanie pocz. obs.');
hold on;
title('Wykres stanu x_2');
xlabel('Czas');
ylabel('x_2');

figure;
plot(tu1, x1(:, 3));
hold on;
plot(tu2, x2(:, 3));
hold on;
% plot(tu1, xhat1(:, 3));
% hold on;
% plot(tu2, xhat2(:, 3));
legend('Stan mierzony przy zerowym stanie pocz. obs.', 'Stan mierzony przy niezerowym stanie pocz. obs.');%, 'Stan obserwowany przy zerowym stanie pocz. obs.', 'Stan obserwowany przy niezerowym stanie pocz. obs.');
hold on;
title('Wykres stanu x_3');
xlabel('Czas');
ylabel('x_3');

figure;
plot(tu1, x1(:, 4));
hold on;
plot(tu2, x2(:, 4));
hold on;
% plot(tu1, xhat1(:, 4));
% hold on;
% plot(tu2, xhat2(:, 4));
legend('Stan mierzony przy zerowym stanie pocz. obs.', 'Stan mierzony przy niezerowym stanie pocz. obs.');%, 'Stan obserwowany przy zerowym stanie pocz. obs.', 'Stan obserwowany przy niezerowym stanie pocz. obs.');
hold on;
title('Wykres stanu x_4');
xlabel('Czas');
ylabel('x_4');

figure;
plot(tu1, u1);
hold on;
plot(tu2, u2);
legend('sygnał sterujący przy zerowym stanie pocz. obs.', 'sygnał sterujący przy niezerowym stanie pocz. obs.');
hold on;
title('Wykres sygnału sterującego');
xlabel('Czas');
ylabel('u');