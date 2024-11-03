#include <TimerOne.h>

long nLeituras = 500;
// 0 <= r <= 1
typedef float (*SinalRefrencia)();
SinalRefrencia sinalReferencia;

int nLeituraAtual = 0;
bool estadoBotao = false;

int degrauUnitarioAtrasoEmLeituras = 20;
float degrauUnitario() {
  return nLeituraAtual >= degrauUnitarioAtrasoEmLeituras ? 1.0f : 0.0f;
}

float testeDeModelo() {
  if (nLeituraAtual < 20) {
    return 0.0f;
  }
  else if (nLeituraAtual < 60) {
    return 1.0f;
  }
  else if (nLeituraAtual < 100) {
    return 0.0f;
  }
  else if (nLeituraAtual < 120) {
    return 1.0f;
  }
  else {
    return 0.0f;
  }
}

void iniciarTeste(SinalRefrencia _sinalReferencia, int _nLeituras, long periodoAmostragemUs) {
  nLeituraAtual = 0;
  sinalReferencia = _sinalReferencia;
  nLeituras = _nLeituras;
  
  Timer1.setPeriod(periodoAmostragemUs);
  Timer1.start();
}
void aplicarSinalReferencia(float r) {
  analogWrite(5, r * 255.f);
}
int lerSaidaSistema() {
  return analogRead(A0);
}
void timerOneInterrupt() {
  if (nLeituraAtual >= nLeituras) {
    Timer1.stop();
    aplicarSinalReferencia(0);
    nLeituraAtual = 0;
    return;
  }
  float r = sinalReferencia();
  aplicarSinalReferencia(r);
  
  float y = lerSaidaSistema();

  Serial.print(r);
  Serial.print(", ");
  Serial.println(y);
  
  nLeituraAtual++;
}

void atualizarEstadoBotao() {
  estadoBotao = !digitalRead(6);
}

void definirSentido(bool horario) {
  digitalWrite(3, horario);
  digitalWrite(4, !horario);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);                 // STBY TB6612
  pinMode(3,OUTPUT);                 // AIN1 TB6612
  pinMode(4,OUTPUT);                 // AIN2 TB6612
  pinMode(5,OUTPUT);                 // PWM ARDUINO UNO
  pinMode(6,INPUT_PULLUP);           // BOTÃO ENTRADA

  digitalWrite(2,HIGH);              // Coloca o driver TB6612 em operação

  definirSentido(true);

  Serial.begin(9600);

  Timer1.initialize(0);
  Timer1.stop();
  Timer1.attachInterrupt(timerOneInterrupt);

  sinalReferencia = degrauUnitario;
}


void loop() {
  // put your main code here, to run repeatedly:
  do {
    atualizarEstadoBotao();
    delay(100);
  }
  while (!estadoBotao);

  iniciarTeste(degrauUnitario, 200, 10000L);
}
