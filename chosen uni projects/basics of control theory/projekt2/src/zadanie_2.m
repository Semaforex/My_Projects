zadanie_1


% Wyznaczanie parametrów modelu z transmitancji dyskretnej
[b, a] = tfdata(Gz, 'v');
b11 = b(2);
b12 = b(3);
a1 = a(2);
a2 = a(3);

N = 80;
u = ones(1, N);
y_diff = zeros(1, N);


% Równanie różnicowe y(t) = -a(2)*y(t-1) - a(3)*y(t-2) + b(2)*u(t-11) + b(3)*u(t-12)
for k = 13:N
    y_diff(k) = -a(2)*y_diff(k-1) - a(3)*y_diff(k-2) + b(2)*u(k-11) + b(3)*u(k-12);
end

Tp = 0.5;
t = 0:Tp:(N-1)*Tp;
y_tf = lsim(Gz, u, t);

disp('Równanie różnicowe:');
disp(['y(t) = ' num2str(-a(2)) '*y(t-1) + ' num2str(-a(3)) '*y(t-2) + ' num2str(b(2)) '*u(t-11) + ' num2str(b(3)) '*u(t-12)']);

fig = figure;
stairs(t(1:end-1), y_diff(2:end), 'b', 'LineWidth', 1.5); hold on;
stairs(t, y_tf, 'r', 'LineWidth', 1.5);
xlabel('Czas [s]');
ylabel('y(t)');
legend('Model różnicowy', 'Transmitancja dyskretna', 'Location', 'best');
title('Porównanie odpowiedzi modelu różnicowego i transmitancji dyskretnej');
grid on;    

filename = fullfile(output_dir, "odpowiedz_skokowa_2.pdf");
exportgraphics(fig, filename, 'ContentType', 'vector', 'BackgroundColor', 'none');
