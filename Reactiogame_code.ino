/*
  Reaktiopeli - 3 LEDiä ja 3 nappia
  Peli nopeutuu jokaisen onnistumisen myötä.
*/

const int ledPins[] = {2, 3, 4}; // LED-pinnit
const int buttonPins[] = {8, 9, 10}; // Nappi-pinnit
int score = 0;
float gameDelay = 1000.0; // Aloitusviive (ms)
const float speedUpFactor = 0.99; // Kuinka paljon peli nopeutuu (1% per kierros)

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP); // Käytetään sisäistä vastusta
  }

  randomSeed(analogRead(0)); // Alustetaan satunnaisluvut
  Serial.println("Peli alkaa! Paina oikeaa nappia kun valo syttyy.");
  delay(2000);
}

void loop() {
  int targetLed = random(0, 3); // Valitaan satunnainen LED (0, 1 tai 2)
  
  digitalWrite(ledPins[targetLed], HIGH); // Sytytetään valo
  
  unsigned long startTime = millis();
  bool correctPressed = false;
  bool gameOver = false;

  // Odotetaan napin painallusta viiveen ajan
  while (millis() - startTime < gameDelay) {
    for (int i = 0; i < 3; i++) {
      if (digitalRead(buttonPins[i]) == LOW) { // Nappi painettu
        if (i == targetLed) {
          correctPressed = true;
          break;
        } else {
          gameOver = true;
          break;
        }
      }
    }
    if (correctPressed || gameOver) break;
  }

  // Jos viive loppui eikä mitään painettu, peli päättyy myös
  if (!correctPressed) {
    gameOver = true;
  }

  if (gameOver) {
    handleGameOver();
  } else {
    // Onnistuminen!
    digitalWrite(ledPins[targetLed], LOW);
    score++;
    gameDelay *= speedUpFactor; // Nopeutetaan peliä
    Serial.print("Pisteet: ");
    Serial.println(score);
    delay(200); // Pieni tauko ennen seuraavaa valoa
  }
}

void handleGameOver() {
  Serial.println("VÄÄRÄ NAPPI TAI LIIAN HIDAS!");
  Serial.print("LOPPUTULOS: ");
  Serial.println(score);
  
  // Vilkutetaan kaikkia valoja merkiksi häviöstä
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) digitalWrite(ledPins[j], HIGH);
    delay(200);
    for (int j = 0; j < 3; j++) digitalWrite(ledPins[j], LOW);
    delay(200);
  }

  // Nollataan peli
  score = 0;
  gameDelay = 1000.0;
  Serial.println("Peli alkaa alusta...");
  delay(2000);
}
