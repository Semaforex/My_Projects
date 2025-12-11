num_mf = num_local;


ymin = h2min;
ymax = h2max;
u_min = F1min;
u_max = F1max;

inverse = ~use_u;

uData = linspace(u_min, u_max, 1000)';
yData = linspace(ymin, ymax, 1000)';

options = genfisOptions("GridPartition", ...
NumMembershipFunctions=num_mf, ...
InputMembershipFunctionType=mf_type);

if inverse
    fis = genfis(yData, uData, options);
else
    fis = genfis(uData, yData, options);
end

mfs = fis.Inputs(1).MembershipFunctions;


if inverse
    x_range = linspace(ymin, ymax, 1000);
else
    x_range = linspace(u_min, u_max, 1000);
end

%% Wykresy f-cji przynależności
if to_plot
    fig_folder = fullfile('wykresy', 'task_2');
    if ~exist(fig_folder, 'dir')
        mkdir(fig_folder);
    end

    figure;
    hold on;
    for i = 1:length(mfs)
        mf = mfs(i);
        y_vals = evalmf(x_range, mf.Parameters, mf.Type);
        plot(x_range, y_vals);
    end
    hold off;
    grid on;

    if inverse
        xlabel('y');
    else
        xlabel('u');
    end
    hold off;
end