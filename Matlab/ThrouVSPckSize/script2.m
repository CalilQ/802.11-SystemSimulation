clc
clear all
close all

%% General

nStas = 15;
offeData = 0.5;
simTime = 1.1;

%% With BA and Aggregation

pckSizBA = [50 100 150 200 250 300 350 400 450 550 800 1500 2000]';
%           1       2       3       4       5
thrBA = [2.73595 2.94828 2.97702 2.83879 3.01358;... % 50
         3.32318 3.85597 3.98135 3.54313 3.8167 ;... % 100
         3.72351 4.0651  4.1731  4.11699 4.15308;... % 150
         4.03993 4.2683  4.34511 4.14544 4.3087 ;... % 200
         4.01795 4.61313 4.56198 4.2999  4.39721;... % 250
         3.98115 4.60296 4.63849 4.50331 4.58159;... % 300
         4.13934 4.6531  4.81707 4.39566 4.56062;... % 350
         4.45114 4.80924 4.7213  4.54212 4.51667;... % 400
         4.05669 4.74829 4.83068 4.66178 4.76117;... % 450
         4.29827 4.84864 4.90158 4.51877 4.60651;... % 550
         4.18217 4.90414 4.82487 4.6205  4.74179;... % 800
         4.48788 4.88108 4.83597 4.59483 4.66798;... % 1500
         4.38234 4.81329 5.26397 4.62366 4.9082];    % 2000
     
 mThrBA = mean(thrBA,2)./nStas;
 effBA = 100*mThrBA./(simTime*offeData);
 
 %% Without BA and Aggregation
 
pckSiz = [50 100 150 200 250 300 350 400 450 550 800 1000 1500 2000]';
%         1       2        3        4        5
thr = [1.6032  1.69039  1.79916  1.27477  1.30552;... % 50
       2.29746 2.552    2.65039  2.49031  2.33039;... % 100
       2.9444  3.08516  3.05757  2.8392   2.15249;... % 150
       3.05912 3.5887   3.51938  3.12951  3.44285;... % 200
       3.1058  3.68738  3.75968  3.62162  3.29966;... % 250
       3.31901 3.86389  3.97105  3.41181  3.01422;... % 300
       3.07658 3.99838  4.01357  3.83754  3.19168;... % 350
       3.54152 4.1109   4.31653  4.06711  3.89414;... % 400
       3.8326  4.17085  4.25785  3.7835   3.99942;... % 450
       3.61218 4.55634  4.42979  4.07436  4.18681;... % 550
       3.98582 4.46505  4.76149  4.30365  4.14553;... % 800
       3.98808 4.49307  4.74395  4.33095  4.4553 ;... % 1000
       4.17532 4.97946  4.9074   4.61574  4.70885;... % 1500
       4.42397 5.22896  5.24795  4.93298  4.92123];   % 2000
 mThr = mean(thr,2)./nStas;
 eff = 100*mThr./(simTime*offeData);
 
 %% Plot
 
 fittedBA = fit(pckSizBA,effBA,'smoothingspline');
 h1 = plot(fittedBA,'b');
 set(h1,'LineWidth',2);
 hold on;
 plot(pckSizBA,effBA,'bo','LineWidth',2);
 legend off;
 
%  fitted = fit(pckSiz,eff,'smoothingspline');
%  h2 = plot(fitted,'r');
%  set(h2,'LineWidth',2);
%  hold on;
%  plot(pckSiz,eff,'ro','LineWidth',2);
%  legend off;
 
 grid on;
 axis tight;
 xlabel('Dados por pacote [Bytes]');
 ylabel('Efici�ncia de Throughput M�dio [%]');
 hold off;
 
 print('-dbmp','effVSPckSz');
 
 %% Save data
 
 save data2