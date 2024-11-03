%compara saída sem filtro e com filtro
filtro = readmatrix("../../Área de Trabalho/putty.log");

t = linspace(0, length(filtro(:, 1))*0.01, length(filtro(:, 1)));
f = linspace(0, 100, length(filtro(:,1)));

subplot(2, 1, 1);
plot(t, filtro(:, 1));
hold on;
plot(t, filtro(:, 2));
hold off;

subplot(2, 1, 2);
fft1 = fft(filtro(:, 1));
plot(f, abs(fft1));
hold on;
fft2 = fft(filtro(:, 2));
plot(f, abs(fft2));
hold off;