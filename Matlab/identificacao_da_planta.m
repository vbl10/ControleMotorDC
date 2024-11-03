% Criar um modelo matemático da planta e executar testes de validação a
% partir de dados reais coletados

testeDegrauUnitario = readmatrix("../../Área de Trabalho/degrauUnitario.log");
testeDeModelo = readmatrix("../../Área de Trabalho/testeModelo.log");

data = iddata(testeDegrauUnitario(:, 2) - 499, testeDegrauUnitario(:, 1), 0.01);
dataValidacao = iddata(testeDeModelo(:, 2) - 499, testeDeModelo(:, 1), 0.01);

% 2 pólos e 0 zeros, estrutura típica de um motor DC
Gest = tfest(data, 2, 0, NaN);

opt = compareOptions;
opt.InitialCondition = 'z';
compare(data, Gest, opt);
%compare(dataValidacao, Gest, opt);