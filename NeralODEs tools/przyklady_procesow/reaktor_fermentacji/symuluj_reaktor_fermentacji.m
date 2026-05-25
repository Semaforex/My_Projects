%% u=Fa, y=T=x7
%% umin = 0; umax = 200
%% dt = 0.5
%% war. pocz.: poniżej

%% w regulacji: wejście Fa, wyjście y=T=x7

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);
kolory = lines;

dt = 0.5;

Fa_dyskr0 = 18;
Fa_dyskr_okres_zmian = 250;
Fa_dyskr_liczba_okresow = 15;
Fa_dyskr_rozbieg = 10;      
Fa_dyskr_min = 0; Fa_dyskr_max = 200;

Fa_dyskr = Fa_dyskr_min + (Fa_dyskr_max - Fa_dyskr_min) * rand(1, Fa_dyskr_liczba_okresow);
Fa_dyskr = kron(Fa_dyskr, ones(1, Fa_dyskr_okres_zmian));
Fa_dyskr = [Fa_dyskr0 * ones(1, Fa_dyskr_rozbieg) Fa_dyskr];

kmax = length(Fa_dyskr);

xpocz = [1000; 0.90467678228155; 12.51524128083789; ...
         29.73892382828279; 3.10695341758232; ...
         29.57321214183856; 27.05393890970931];

Tdyskr = zeros(1,kmax);
x_all_dyskr = zeros(7, kmax);

xcont=[];
for k = 1:kmax

    tpsym = (k-1)*dt;
    tksym = k*dt;

    Fag = Fa_dyskr(k);

    u = [51, 51, 25, 60, Fag];

    [t,x] = ode15s(@(t,x) sf_bioc_model(t,x,u), ...
                   [tpsym tksym], xpocz);

    x_final_step = x(end, :)';
    
    x_all_dyskr(:, k) = x_final_step;    

    xpocz = x(end,:)';
    Tdyskr(k) = xpocz(6);

    xcont=[xcont; x];
end

figure;
subplot(2,1,1);
stairs(Fa_dyskr);
ylabel('Fag=x7');
xlabel('k');

subplot(2,1,2);
stairs(Tdyskr);
ylabel('T');
xlabel('k');

figure;
tiledlayout(2, 4, 'TileSpacing', 'compact', 'Padding', 'compact');
for i = 1:7
    nexttile;    
    plot(x_all_dyskr(i, :),'Color',kolory(i, :));    
    xlabel('k');
    ylabel(sprintf('x_{%d}', i));
    grid on;
end

