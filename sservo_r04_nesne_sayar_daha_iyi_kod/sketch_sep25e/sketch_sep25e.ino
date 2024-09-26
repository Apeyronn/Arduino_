#include <Servo.h>
#include <LiquidCrystal.h> // LCD kütüphanesi

// Pin ayarları
const int trigPin = 6; // Trig pinini 6 olarak ayarladık
const int echoPin = 7; // Echo pinini 7 olarak ayarladık
const int ledPin = 10; // LED için pin
const int buzzerPin = 9; // Buzzer için pin
const int servoPin = 8; // Servo motor için pin
const int threshold = 60; // Mesafe eşiği (cm)
const unsigned long detectionDuration = 1250; // 1 saniye (milisaniye cinsinden)

const int maxObjects = 3; // Algılanabilecek maksimum nesne sayısı
const unsigned long maxDuration = 20000; // 30 saniye (milisaniye cinsinden)

// LCD bağlantıları
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // RS, E, D4, D5, D6, D7
Servo myServo; // Servo motor nesnesi

// Değişkenler
bool objectDetected = false; // Nesne algı durumu
int currentAngle = 0; // Servo motorun mevcut açısı
unsigned long detectionStartTime = 0; // Nesnenin algılanmaya başladığı zaman
int objectCounter = 0; // Algılanan nesnelerin sayısı
bool isObjectPrinted = false; // Nesnenin ekrana yazılıp yazılmadığını kontrol eder
bool stopMotor = false; // Motorun durma durumu
unsigned long startMillis; // Başlangıç zamanı için değişken

void setup() {
  Serial.begin(9600); // Seri iletişimi başlat
  pinMode(trigPin, OUTPUT); // Trig pinini çıkış olarak ayarla
  pinMode(echoPin, INPUT);  // Echo pinini giriş olarak ayarla
  pinMode(ledPin, OUTPUT);   // LED pinini çıkış olarak ayarla
  pinMode(buzzerPin, OUTPUT); // Buzzer pinini çıkış olarak ayarla

  myServo.attach(servoPin); // Servo motoru bağla
  myServo.write(currentAngle); // Başlangıç açısı

  lcd.begin(16, 2); // LCD boyutunu ayarla
  lcd.print("MERHABA"); // Başlangıçta mesajı yazdır

  startMillis = millis(); // Başlangıç zamanını al
}

void loop() {
  // Eğer 30 saniye geçmişse sistemi durdur
  if (millis() - startMillis >= maxDuration) {
    stopMotor = true; // Motoru durdur
  }

  if (stopMotor) {
    lcd.clear();
    lcd.print("Motor durdu");
    delay(15000); // 15 saniye bekle
    lcd.clear();
    lcd.print("Sistem kapatiliyor");
    delay(2000); // 2 saniye bekle
    return; // Eğer motor durduysa geri dön ve başka işlem yapma
  }

  int distance = checkDistance(); // Mesafeyi kontrol et

  if (distance < threshold) { // Eğer nesne 30 cm'den yakınsa
    if (!objectDetected) {
      objectDetected = true;
      detectionStartTime = millis(); // Algılanma süresini başlat
      isObjectPrinted = false; // Nesnenin yazdırılmadığını işaretle
    }

    // Eğer nesne 1 saniyeden uzun algılanırsa
    if (millis() - detectionStartTime > detectionDuration && !isObjectPrinted) {
      objectCounter++; // Nesne sayısını arttır
      lcd.clear();
      lcd.print("Nesne ");
      lcd.print(objectCounter); // Algılanan nesne numarasını yazdır
      lcd.setCursor(0, 1);
      lcd.print("Algilandi");
      delay(1000); // Ekranda 1 saniye boyunca göster
      isObjectPrinted = true; // Nesne yazdırıldı olarak işaretle

      if (objectCounter >= maxObjects) {
        stopMotor = true; // 3 nesne algılandıysa motoru durdur
      }
    }

    digitalWrite(ledPin, HIGH); // LED'i yak
    tone(buzzerPin, 1000); // Buzzer'ı çaldır
  } else if (objectDetected) {
    // Eğer nesne algılandı ve sonra uzaklaştıysa
    objectDetected = false;
    digitalWrite(ledPin, LOW); // LED'i kapat
    noTone(buzzerPin); // Buzzer'ı kapat
    lcd.clear();
    lcd.print("Taramaya devam");
  }

  // Eğer nesne algılanmadıysa servo motoru döndürmeye devam et
  if (!objectDetected && !stopMotor) {
    scanArea(); // Servo motoru döndür ve alanı tara
  }
}

void scanArea() {
  // Servo motoru önce 0'dan 90 dereceye döndür
  for (int angle = currentAngle; angle <= 90; angle += 1) {
    myServo.write(angle);
    delay(15); // Servo motor hareketi için bekle
    currentAngle = angle; // Mevcut açıyı güncelle
    if (checkDistance() < threshold) { // Eğer mesafe algılanırsa döngüyü durdur
      return;
    }
  }

  // Servo motoru 90'dan 0 dereceye geri döndür
  for (int angle = currentAngle; angle >= 0; angle -= 1) {
    myServo.write(angle);
    delay(15); // Servo motor hareketi için bekle
    currentAngle = angle; // Mevcut açıyı güncelle
    if (checkDistance() < threshold) { // Eğer mesafe algılanırsa döngüyü durdur
      return;
    }
  }
}

int checkDistance() {
  // Trig pinini LOW yap
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trig pinini HIGH yap ve 10 mikro saniye bekle
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Echo pininden gelen süreyi oku
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Mesafe cm cinsinden

  // Mesafeyi seri port üzerinden yazdır
  Serial.print("Mesafe: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance; // Mesafeyi geri döndür
}
