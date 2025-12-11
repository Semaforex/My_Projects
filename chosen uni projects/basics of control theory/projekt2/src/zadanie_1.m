clear;
close all;
output_dir = './wykresy';

% Parametry modelu procesu
K0 = 3.5;
T0 = 5;
T1 = 2.16;
T2 = 5.52;
Tp = 0.5;

s = tf('s');
% Ciągła transmitancja obiektu
G_s = K0 * exp(-T0*s) / ((T1*s+1)*(T2*s+1));

% Dyskretyzacja transmitancji ciągłej
Gz = c2d(G_s, Tp, 'zoh');

disp('Transmitancja dyskretna G(z):')
Gz

fig = figure;

step(Gz, 'r', G_s, 'b--');
legend('transmitancja dyskretna','transmitancja ciagla', 'Location', 'best');
xlabel('Czas [s]');
ylabel('Odpowiedz skokowa');
title('Porownanie odpowiedzi skokowej');


filename = fullfile(output_dir, "odpowiedz_skokowa.pdf");
exportgraphics(fig, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');


% Obliczanie wzmocnienia statycznego dla G(s) i G(z)
K_stat_s = dcgain(G_s);
K_stat_z = dcgain(Gz);
fprintf('Wzmocnienie statyczne G(s): %.4f\n', K_stat_s);
fprintf('Wzmocnienie statyczne G(z): %.4f\n', K_stat_z);
