#ifndef FILTRO_FIR_H
#define FILTRO_FIR_H

template <int TAM>
class FiltroFIR
{
private:
    const float* coefs;
    float amostras[TAM] = { 0.0f };
    int k = 0;

public:
    FiltroFIR(const float* coefs): coefs(coefs) {}
    float atualizar(float amostra)
    {
        float val = 0.0f;
        amostras[k] = amostra;
        for (int i = 0; i < TAM; i++)
        {
            val += amostras[(k - i + TAM) % TAM] * coefs[i];
        }
        k = (k + 1) % TAM;
        return val;
    }
};

#endif