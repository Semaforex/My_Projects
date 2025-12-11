zadanie_2

s = tf('s');
% Ciągła transmitancja obiektu
G_s = K0 * exp(-T0*s) / ((T1*s+1)*(T2*s+1));

% Wyznaczanie marginesu amplitudy i fazy oraz parametrów krytycznych
[Gm, Pm, Wcg, Wcp] = margin(G_s);
Kk = Gm;             
Tk = 2*pi/Wcg;

fig = figure;
t = 0:0.1:60;
y = step(feedback(Kk*G_s, 1), t);
plot(t, y);
xlabel('t [s]');
ylabel('y(t)');
title('Odpowiedz krytyczna przy wzmocnieniu krytycznym K_k');
grid on;

filename = fullfile(output_dir, "KK.pdf");
exportgraphics(fig, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');

fprintf('Wzmocnienie krytyczne Kk = %.4f\n', Kk);
fprintf('Okres oscylacji krytycznej Tk = %.4f\n', Tk);

% Wyznaczanie parametrów regulatora PID metodą Zieglera-Nicholsa
K = 0.6 * Kk;      
Ti = 0.5 * Tk;      
Td = 0.12 * Tk;     

fprintf('Parametry regulatora PID:\n');
fprintf('K = %.4f\n', K);
fprintf('Ti = %.4f s\n', Ti);
fprintf('Td = %.4f s\n', Td);


% Wyznaczanie parametrów cyfrowego regulatora PID
r2 = K*Td/Tp;
r1 = K*(Tp/(2*Ti)-2*Td/Tp-1);
r0 = K*(1+Tp/(2*Ti)+Td/Tp);

fprintf('Parametry cyfrowego regulatora PID:\n');
fprintf('r2 = %.4f\n', r2);
fprintf('r1 = %.4f\n', r1);
fprintf('r0 = %.4f\n', r0);
