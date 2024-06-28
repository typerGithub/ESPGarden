/* EspWeather — свободный проект: вы можете перераспространять его и/или изменять его на условиях Стандартной общественной лицензии GNU в том виде, в каком он был опубликована Фондом свободного программного обеспечения; либо версии 3 лицензии, либо (по вашему выбору) любой более поздней версии.
EspWeather распространяется в надежде, что он будет полезным, но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной общественной лицензии GNU.
Вы должны были получить копию Стандартной общественной лицензии GNU вместе с этим проектом. Если это не так, см. <https://www.gnu.org/licenses/>. */

/* EspWeather is free project: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
EspWeather is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with EspWeather. If not, see <https://www.gnu.org/licenses/>. */

int relay = YOUR_RELAY_PIN;
int dht11 = 0;
int moisture = 0;
int moisture2 = 0;

#include "DHT.h"

DHT dht_32(32,DHT11);

#include <WiFi.h>
  #include <WiFiClientSecure.h>
  #include <UniversalTelegramBot.h>
  #define WIFI_SSID "SSID_YOUR_WIFI"
  #define WIFI_PASSWORD "PASSWORD_YOUR_WIFI"
  #define BOT_TOKEN "YOUR_BOT_TOKEN"
  const unsigned long BOT_MTBS = 1000;
  WiFiClientSecure secured_client;
  UniversalTelegramBot bot(BOT_TOKEN, secured_client);
  unsigned long bot_lasttime;
  void handleNewMessages(int numNewMessages)
  {
    Serial.print("handleNewMessages ");
    Serial.println(numNewMessages);

    for (int i = 0; i < numNewMessages; i++)
    {
      String chat_id = bot.messages[i].chat_id;
      String text = bot.messages[i].text;
      String from_name = bot.messages[i].from_name;
      if (from_name == "")
        from_name = "Guest";
     if(text == "Включить свет"){
      pinMode(relay, OUTPUT);
     digitalWrite(relay, 1);
    bot.sendMessage(chat_id, "Включено", "");
   }
    if(text == "Выключить свет"){
      pinMode(relay, OUTPUT);
     digitalWrite(relay, 0);
    bot.sendMessage(chat_id, "Выключено", "");
   }
    if(text == "Температура"){
      bot.sendMessage(chat_id, (String("Температура  ") + String(String(dht11)) + String("°C")), "");
   }
    if(text == "Влажность воздуха"){
      bot.sendMessage(chat_id, (String("Влажность  ") + String(String(moisture)) + String("%")), "");
   }
    if(text == "Влажность почвы"){
      bot.sendMessage(chat_id, (String("Влажность почвы ") + String(String(moisture2)) + String("%")), "");
   }
    if(text == "Все показания"){
      bot.sendMessage(chat_id, (String("Температура  ") + String(String(dht11)) + String("°C")), "");
    bot.sendMessage(chat_id, (String("Влажность почвы ") + String(String(moisture2)) + String("%")), "");
    bot.sendMessage(chat_id, (String("Влажность воздуха ") + String(String(moisture)) + String("%")), "");
   }
  if (dht11 >= 17) {
    pinMode(relay, OUTPUT);
     digitalWrite(relay, 0);
  }
 }
 }


void setup() {
  dht_32.begin();

  pinMode(35, INPUT);
  Serial.begin(9600);
   Serial.println();
    Serial.print("Connecting to Wifi SSID ");
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
    Serial.print("WiFi connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Retrieving time: ");
    configTime(0, 0, "pool.ntp.org");
    time_t now = time(nullptr);
    while (now < 24 * 3600)
    {
      Serial.print(".");
      delay(100);
      now = time(nullptr);
    }
    Serial.println(now);



}

void loop() {
    dht11 = dht_32.readTemperature();
    moisture = dht_32.readHumidity();
    moisture2 = digitalRead(35);
     if (millis() - bot_lasttime > BOT_MTBS)
        {
          int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
          while (numNewMessages)
          {
            Serial.println("got response");
            handleNewMessages(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
          }
          bot_lasttime = millis();
        }

    if (dht11 < 17) {
      pinMode(relay, OUTPUT);
       digitalWrite(relay, 1);
    }

}