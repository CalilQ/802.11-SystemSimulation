clc
clear all
close all

%% General

offeData = 0.5;
simTime = 1.1;

%% With BA and Aggregation

distBA = [10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 170]';

thrBA = [0.510492 0.59183 0.489558 0.457721 0.424103;... % 10
         0.586083 0.59183 0.489558 0.492826 0.424103;... % 20
         0.558422 0.50041 0.382266 0.639481 0.424103;... % 30
         0.415973 0.50258 0.416761 0.476716 0.424103;... % 40
         ;... % 50
         ;... % 60
         ;... % 70
         ;... % 80
         ;... % 90
         ;... % 100
         ;... % 110
         ;... % 120
         ;... % 130
         ;... % 140
         ;... % 150
         ];   % 170

 mThrBA = mean(thrBA,2);
 effBA = 100*mThrBA./(simTime*offeData);

%% Without BA and Aggregation

dist = [10 20 30 40 50 60 70 80 90 100 110 120 130 140 150 170]';

thr = [0.510492 0.583842 0.489558 0.457721 0.424103;... % 10
    0.586083 0.583842 0.489558 0.455646 0.424103;... % 20
    0.546528 0.492464 0.398259 0.504931 0.424103;... % 30
    0.419275 0.43051  0.500341 0.569466 0.527632;... % 40
    0.476912 0.491601 0.488292 0.551492 0.471635;... % 50
    0.495984 0.470208 0.468402 0.495956 0.410839;... % 60
    0.367988 0.559444 0.447924 0.526417 0.555896;... % 70
    0.463936 0.397009 0.495158 0.567333 0.397512;... % 80
    0.49596  0.455332 0.471784 0.53781  0.565315;... % 90
    0.239992 0.49554  0.415698 0.62451  0.567007;... % 100
    0.087992 0.495861 0.383734 0.503403 0.463964;... % 110
    0.015999 0.311982 0.359577 0.55497  0.447651;... % 120
    0        0.223978 0.215992 0.582185 0.527966;... % 130
    0        0.119997 0.159985 0.223989 0.39998;... % 140
    0        0        0.175999 0.023999 0.575973;... % 150
    0        0        0        0        0.119999];   % 170

 mThr = mean(thr,2);
 eff = 100*mThr./(simTime*offeData);

%% Plot

 fitted = fit(dist,eff,'smoothingspline');
 h2 = plot(fitted,'r');
 set(h1,'LineWidth',2);
 hold on;
 plot(dist,eff,'ro','LineWidth',2);

 legend off;
 grid on;
 axis tight;
 xlabel('Dist�ncia da esta��o ao ponto de acesso [m]');
 ylabel('Efici�ncia de Throughput M�dio [%]');
 hold off;
 
 print('-dbmp','effVSdist');