#include "ControladorPID.h"
#include "FiltroFIR.h"
#include "SinalReferencia.h"

#define PIN_TB6612_STBY 2
#define PIN_TB6612_AIN1 3
#define PIN_TB6612_AIN2 4
#define PIN_TB6612_PWM 5
#define PIN_BOTAO 6
#define PIN_TACOGERADOR A0

constexpr float frequenciaAmostragem = 100.0f;
constexpr float periodoAmostragem_s = 1.0f / frequenciaAmostragem;
constexpr unsigned long periodoAmostragem_us = (unsigned long)(periodoAmostragem_s * 1000000.0f);

// 0 < r < 1
float sinalReferencia()
{
    return 0.2f;
}

const float firCoefs[] = {
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
    1.0f/8.0f,
};
FiltroFIR<8> filtro(firCoefs);
ControladorPID pid(
    1.384, 
    33.8766, 
    -0.0014862, 
    60.0089,   
    periodoAmostragem_s
);

void definirSentido(bool horario)
{
    digitalWrite(PIN_TB6612_AIN1, horario);
    digitalWrite(PIN_TB6612_AIN2, !horario);
}

void controlar()
{
    float y = ((float)analogRead(PIN_TACOGERADOR) - 499.0f) / 188.8f;
    float yf = filtro.atualizar(y);     //y filtrado
    float ef = sinalReferencia() - yf;  //erro filtrado
    float e = sinalReferencia() - y;    //erro sem filtro (aplicado ao controlador)
    float c = pid.atualizar(e);
    analogWrite(PIN_TB6612_PWM, c * 255.0f);

    Serial.print(y);
    Serial.print(", ");
    Serial.print(yf);
    Serial.print(", ");
    Serial.print(ef);
    Serial.print(", ");
    Serial.print(c);
    Serial.println();

}

void setup()
{
    Serial.begin(9600);

    pinMode(2, OUTPUT);       // STBY TB6612
    pinMode(3, OUTPUT);       // AIN1 TB6612
    pinMode(4, OUTPUT);       // AIN2 TB6612
    pinMode(5, OUTPUT);       // PWM ARDUINO UNO
    pinMode(6, INPUT_PULLUP); // BOTÃO ENTRADA

    digitalWrite(2, HIGH); // Coloca o driver TB6612 em operação

    definirSentido(true);

    Serial.begin(9600);
}

bool bEstadoPrograma = false;
bool bUltimoEstadoPrograma = false;
unsigned long tp1 = 0;
void loop()
{
    if (!digitalRead(PIN_BOTAO)) {
        bEstadoPrograma = !bEstadoPrograma;
        while (!digitalRead(PIN_BOTAO)) delay(100);
    }
    if (bEstadoPrograma) {
        if (!bUltimoEstadoPrograma) {
            bUltimoEstadoPrograma = true;
            tp1 = micros();
        }
        if (micros() - tp1 >= periodoAmostragem_us) {
            tp1 += periodoAmostragem_us;
            controlar();
        }
    }
    else if (bUltimoEstadoPrograma) {
        bUltimoEstadoPrograma = false;
        analogWrite(PIN_TB6612_PWM, 0);
    }
}
