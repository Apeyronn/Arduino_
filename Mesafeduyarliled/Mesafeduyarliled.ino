// Pin tanımlamaları
const int trigPin = 7;  // HC-SR04 TRIG pini
const int echoPin = 6;  // HC-SR04 ECHO pini
const int ledPin = 8;   // LED'in bağlı olduğu pin

long duration;
int distance;

void setup() {
  // Pin modları
  pinMode(trigPin, OUTPUT);  // TRIG pini çıkış
  pinMode(echoPin, INPUT);   // ECHO pini giriş
  pinMode(ledPin, OUTPUT);   // LED pini çıkış
  
  // Seri haberleşme
  Serial.begin(9600);        // Seri monitörü başlat
}

void loop() {
  // TRIG pinini LOW yapıp bekleyelim
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // TRIG pinine 10 mikrosaniyelik HIGH sinyali gönder
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Echo pinindeki süreyi ölç
  duration = pulseIn(echoPin, HIGH);
  
  // Süreyi mesafeye çevir
  distance = duration * 0.034 / 2;  // Mesafeyi cm cinsine çevir

  // Mesafeyi seri monitöre yazdır
  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Eğer mesafe 20 cm'den küçükse, LED yanıp sönecek
  if (distance < 20) {
    // LED'i yak (HIGH durumu)
    digitalWrite(ledPin, HIGH);
    delay(500); // 0.5 saniye bekle
    
    // LED'i söndür (LOW durumu)
    digitalWrite(ledPin, LOW);
    delay(500); // 0.5 saniye bekle
  } else {
    // Eğer mesafe 20 cm'den büyükse LED kapalı kalacak
    digitalWrite(ledPin, LOW);
  }
}
