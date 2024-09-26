#include <Servo.h>

Servo myServo; // Servo nesnesini tanımlayın
int ledPin = 13; // LED için pin
int servoPin = 9; // Servo motoru için pin
int trigPin = 2; // HC-SR04 Trig pin
int echoPin = 3; // HC-SR04 Echo pin
int angle = 0; // Servo açısı
const int ledDelay = 500; // LED yanıp sönme süresi
const int threshold = 30; // Mesafe eşiği (cm)

void setup() {
    myServo.attach(servoPin); // Servo motor pinini ayarla
    pinMode(ledPin, OUTPUT); // LED pinini çıkış olarak ayarla
    pinMode(trigPin, OUTPUT); // Trig pinini çıkış olarak ayarla
    pinMode(echoPin, INPUT); // Echo pinini giriş olarak ayarla
    Serial.begin(9600); // Serial iletişimini başlat
}

void loop() {
    // Servo motoru döndür
    for (angle = 0; angle <= 180; angle += 1) {
        myServo.write(angle); // Servo motor açıyı ayarla
        delay(15); // Motorun hareket etmesi için bekle
        checkDistance(); // Mesafeyi kontrol et
    }
    for (angle = 180; angle >= 0; angle -= 1) {
        myServo.write(angle); // Servo motor açıyı ayarla
        delay(15); // Motorun hareket etmesi için bekle
        checkDistance(); // Mesafeyi kontrol et
    }
}

void checkDistance() {
    // HC-SR04 ile mesafe ölçümü
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2; // Mesafe cm cinsinden
    Serial.print("Mesafe: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Mesafeye göre LED'i kontrol et
    if (distance < threshold) { // Eğer mesafe 30 cm'den azsa
        blinkLED(); // LED'i yanıp söndür
    } else {
        digitalWrite(ledPin, LOW); // LED'i kapat
    }
}

void blinkLED() {
    digitalWrite(ledPin, HIGH); // LED'i yak
    delay(ledDelay); // Belirtilen süre kadar bekle
    digitalWrite(ledPin, LOW); // LED'i kapat
    delay(ledDelay); // Belirtilen süre kadar bekle
}
