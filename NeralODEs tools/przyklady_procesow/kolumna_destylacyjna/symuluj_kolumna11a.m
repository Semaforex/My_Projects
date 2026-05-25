%% okres próbkowania modelu w regulacji T=1
%% zakres sterowania u=1...10
%% warunek pocz¹tkowy u0, x0 - poni¿ej

%% w regulacji: wejœcie u=R, wyjœcie y=x1

clear all;
grubosc=1.25; set(groot,'DefaultLineLineWidth',grubosc); set(groot,'DefaultStairLineWidth',grubosc);

dt=1;

u_okres_zmian=50;
u_liczba_okresow=20;
u_rozbieg=10;
u_min=1; u_max=10;

%warunek pocz¹tkowy
u0=3;
x0(1:32)=[0.93541941614016
   0.90052553715795
   0.86229645132283
   0.82169940277993
   0.77999079584355
   0.73857168629759
   0.69880490932694
   0.66184253445732
   0.62850777645505
   0.59925269993058
   0.57418567956453
   0.55314422743545
   0.53578454439850
   0.52166550959767
   0.51031495114413
   0.50127509227528
   0.49412891686784
   0.48544992019184
   0.47420248108803
   0.45980349896163
   0.44164297270225
   0.41919109776836
   0.39205549194059
   0.36024592617390
   0.32407993023343
   0.28467681591738
   0.24320921343484
   0.20181568276528
   0.16177269003094
   0.12514970961746
   0.09245832612765
   0.06458317697321];

u_dyskr=u_min+(u_max-u_min)*rand(1,u_liczba_okresow);
u_dyskr=kron(u_dyskr, ones(1,u_okres_zmian));
u_dyskr=[u0*ones(1,u_rozbieg) u_dyskr];

kmax=length(u_dyskr);
x1_dyskr = zeros(1,kmax);
x_all_dyskr = zeros(32, kmax);

options = odeset('RelTol',1e-6,'AbsTol',1e-8);

xc=[];
for k=1:kmax
    tpsym = (k-1)*dt;
    tksym = k*dt;

    u = u_dyskr(k);
    [~, x_ode] = ode15s(@(t,x) distill_siso(t,x,u), ...
        [tpsym tksym], x0', options);
    
    x1_dyskr(k)=x_ode(end,1);
    x_all_dyskr(:, k) = x_ode(end, :)';

    x0 = x_ode(end,:)';

    xc=[xc; x_ode];
end

figure;
subplot(2,1,1); stairs(u_dyskr); xlabel('k'); ylabel('u');
subplot(2,1,2); plot(x1_dyskr); xlabel('k'); ylabel('x1');
disp(sprintf('u=%2.8e...%2.8e, zakres=%2.8e',min(u_dyskr),max(u_dyskr),max(u_dyskr)-min(u_dyskr)));
disp(sprintf('x1=%2.8e...%2.8e, zakres=%2.8e',min(x1_dyskr),max(x1_dyskr),max(x1_dyskr)-min(x1_dyskr)));

figure
plot(x_all_dyskr'); 
xlabel('k'); ylabel('x_i'); 
title('Wszystkie zmienne stanu (x1,..., x32)');
grid on;
