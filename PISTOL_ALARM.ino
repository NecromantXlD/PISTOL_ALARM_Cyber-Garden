#include <DHT.h>
#define DHTPIN 13
#define DHTTYPE DHT11
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

bool alarmEnabled = true; 
int sensorPin = 34;
int value = 0;
WiFiServer server(80);
int alarmHour = 7;   
int alarmMinute = 0; 

String header;

LiquidCrystal lcd(22, 21, 5, 18, 15, 19);
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "iPhone (Nikita)";
const char* password = "11111111";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec);

#define SPEAKER_PIN 4  

const int ledcChannel = 0;  
const int frequency = 3000;  
const int resolution = 8;   

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.begin(16, 2);
  lcd.clear();

  Serial.print("Подключение к ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Подключено к WiFi.");
  Serial.println("IP-адрес: ");
  Serial.println(WiFi.localIP());
  server.begin();

  timeClient.begin();

  ledcSetup(ledcChannel, frequency, resolution);

  ledcAttachPin(SPEAKER_PIN, ledcChannel);
  ledcWrite(ledcChannel, 255);  
  pinMode(sensorPin, INPUT);
}

void loop() {

if (header.indexOf("GET /?hour=") >= 0 && header.indexOf("&minute=") >= 0) {
  int pos1 = header.indexOf('=');
  int pos2 = header.indexOf('&');
  alarmHour = header.substring(pos1 + 1, pos2).toInt();

  int pos3 = header.indexOf("minute=");
  int pos4 = header.indexOf('&', pos3);
  alarmMinute = header.substring(pos3 + 7, pos4).toInt();
}
value = analogRead(sensorPin);

  Serial.println(value);

  delay(1000);

  timeClient.update();
  String dateTime = timeClient.getFormattedTime();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error reading DHT11 sensor data");
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print(temperature, 0);
  lcd.print("'C");
  lcd.setCursor(6, 0);
  lcd.print("TIME ");
  lcd.setCursor(5, 1);
  lcd.print(dateTime.substring(0, 5));
  lcd.setCursor(13, 0);
  lcd.print(humidity, 0);
  lcd.print("%");

  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  if (currentHour == alarmHour && currentMinute == alarmMinute && alarmEnabled) {
    Serial.println("Alarm triggered!");

    tone(SPEAKER_PIN, frequency);

  if (value >= 800) {
    noTone(SPEAKER_PIN);
    alarmEnabled = false;
    }
  }

 
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    

          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");

            
            client.println("</head><body><h1>Alarm Clock</h1>");
            client.println("<p>Set Alarm Time:</p>");

         
            client.println("<label for=\"hourSlider\">Select Hour:</label>");
            client.println("<input type=\"range\" min=\"0\" max=\"23\" class=\"slider\" id=\"hourSlider\" value=\"" + String(alarmHour) + "\">");
            client.println("<p>Selected Hour: <span id=\"hourValue\">" + String(alarmHour) + "</span></p>");

    
            client.println("<label for=\"minuteSlider\">Select Minute:</label>");
            client.println("<input type=\"range\" min=\"0\" max=\"59\" class=\"slider\" id=\"minuteSlider\" value=\"" + String(alarmMinute) + "\">");
            client.println("<p>Selected Minute: <span id=\"minuteValue\">" + String(alarmMinute) + "</span></p>");

       
            client.println("<script>");
            client.println("var hourSlider = document.getElementById(\"hourSlider\");");
            client.println("var minuteSlider = document.getElementById(\"minuteSlider\");");
            client.println("var hourValue = document.getElementById(\"hourValue\");");
            client.println("var minuteValue = document.getElementById(\"minuteValue\");");
            client.println("hourValue.innerHTML = hourSlider.value;");
            client.println("minuteValue.innerHTML = minuteSlider.value;");
            client.println("hourSlider.oninput = function() {");
            client.println("  hourValue.innerHTML = this.value;");
            client.println("}");
            client.println("minuteSlider.oninput = function() {");
            client.println("  minuteValue.innerHTML = this.value;");
            client.println("}");
            client.println("$.ajaxSetup({timeout:1000});");
            client.println("function setAlarm() {");
            client.println("  var selectedHour = hourSlider.value;");
            client.println("  var selectedMinute = minuteSlider.value;");
            client.println("  $.get(\"/?hour=\" + selectedHour + \"&minute=\" + selectedMinute + \"&\");");
            client.println("}");
            client.println("</script>");

            client.println("<button onclick=\"setAlarm()\">Set Alarm</button>");

            client.println("</body></html>");


            if (header.indexOf("GET /?hour=") >= 0 && header.indexOf("&minute=") >= 0) {
              int pos1 = header.indexOf('=');
              int pos2 = header.indexOf('&');
              alarmHour = header.substring(pos1 + 1, pos2).toInt();

              int pos3 = header.indexOf("minute=");
              int pos4 = header.indexOf('&', pos3);
              alarmMinute = header.substring(pos3 + 7, pos4).toInt();
            }


            client.println();

            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }

    header = "";
    alarmEnabled = true;

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}