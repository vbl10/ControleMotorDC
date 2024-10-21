void definirDirecao(bool horario) {
  digitalWrite(3, horario);
  digitalWrite(4, !horario);
}

// 0 <= r <= 1
void aplicarSinalReferencia(float r) {
  analogWrite(5, r * 255.f);
}

bool estadoBotao = false;
void atualizarEstadoBotao() {
  estadoBotao = !digitalRead(6);
}

int lerSaidaSistema() {
  return analogRead(A0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);                 // STBY TB6612
  pinMode(3,OUTPUT);                 // AIN1 TB6612
  pinMode(4,OUTPUT);                 // AIN2 TB6612
  pinMode(5,OUTPUT);                 // PWM ARDUINO UNO
  pinMode(6,INPUT_PULLUP);           // BOTÃO ENTRADA
  digitalWrite(2,HIGH);              // Coloca o driver TB6612 em operação
  definirDirecao(true);                         // Seta direção de acordo com o valor de "dir"
  Serial.begin(115200);              // Serial initialize
}

const int nLeituras = 1000;
const int periodoAmostragemUs = 1000;
void loop() {
  // put your main code here, to run repeatedly:
  do {
    atualizarEstadoBotao();
    delay(100);
  }
  while (!estadoBotao);

  aplicarSinalReferencia(1.0f);
  for (int i = 0; i < nLeituras; i++) {
    Serial.println(lerSaidaSistema());
    delayMicroseconds(periodoAmostragemUs);
  }
  aplicarSinalReferencia(0.0f);

}
