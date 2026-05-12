%% Generate Spiral Data

function data = generate_siparal_data(t, noise_std)
    x = sin(t).*exp(-0.1*t);
    y = cos(t).*exp(-0.1*t);
    x = x + noise_std .* randn(size(x));
    y = y + noise_std .* randn(size(y));
    data = [x; y];
end

numTimeSteps = 1000;
t = linspace(0, 10, numTimeSteps);
data = generate_siparal_data(t, 0.01);
xTrain = data;
x0 = data(:,1);

%% Plot the data
figure;
plot(data(1,:), data(2,:));
xlabel('x');
ylabel('y');
title('Noisy Spiral Data');
grid on;

%% ODE Function Definition

neuralOdeParameters = struct;

state_size = size(data, 1);
hidden_size = 50;

neuralOdeParameters.fc1 = struct;
sz = [hidden_size, state_size];
neuralOdeParameters.fc1.Weights = initializeGlorot(sz, hidden_size, state_size);
neuralOdeParameters.fc1.Bias = initializeZeros([hidden_size 1]);

neuralOdeParameters.fc2 = struct;
sz = [state_size, hidden_size];
neuralOdeParameters.fc2.Weights = initializeGlorot(sz, state_size, hidden_size);
neuralOdeParameters.fc2.Bias = initializeZeros([state_size 1]);

neuralOdeParameters.fc1
neuralOdeParameters.fc2

%% Hyperparameters

gradDecay = 0.9;
sqGradDecay = 0.999;
learnRate = 0.002;

numIter = 1000;
miniBatchSize = 200;

plotFrequency = 50;

neuralOdeTimesteps = 40;
dt = t(2);
timesteps = (0:neuralOdeTimesteps)*dt;
averageGrad = [];
averageSqGrad = [];

monitor = trainingProgressMonitor(Metrics="Loss",Info=["Iteration","LearnRate"],XLabel="Iteration");

%% Training loop

numTrainingTimesteps = numTimeSteps;
trainingTimesteps = 1:numTrainingTimesteps;
plottingTimesteps = 2:numTimeSteps;

iteration = 0;

while iteration < numIter && ~monitor.Stop
    iteration = iteration + 1;

    % Create batch
    [X, targets] = createMiniBatch(numTrainingTimesteps, neuralOdeTimesteps, miniBatchSize, xTrain);

    % Evaluate network and compute loss and gradients
    [loss,gradients] = dlfeval(@modelLoss,timesteps,X,neuralOdeParameters,targets);

    % Update network
    [neuralOdeParameters,averageGrad,averageSqGrad] = adamupdate(neuralOdeParameters,gradients,averageGrad,averageSqGrad,iteration,...
        learnRate,gradDecay,sqGradDecay);

    % Plot loss
    recordMetrics(monitor,iteration,Loss=loss);

    % Plot predicted vs. real dynamics
    if mod(iteration,plotFrequency) == 0  || iteration == 1

        % Use ode45 to compute the solution 
        y = dlode45(@odeModel,t,dlarray(x0),neuralOdeParameters,DataFormat="CB");

        plot(xTrain(1,plottingTimesteps),xTrain(2,plottingTimesteps),"r--")

        hold on
        plot(y(1,:),y(2,:),"b-")
        hold off

        xlabel("x(1)")
        ylabel("x(2)")
        title("Predicted vs. Real Dynamics")
        legend("Training Ground Truth", "Predicted")

        drawnow
    end
    updateInfo(monitor,Iteration=iteration,LearnRate=learnRate);
    monitor.Progress = 100*iteration/numIter;
end
