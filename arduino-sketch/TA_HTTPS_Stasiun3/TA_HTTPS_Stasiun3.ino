#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "NimBLEDevice.h"
#include "NimBLEBeacon.h"
#include "NimBLEEddystoneURL.h"

// UUID 1 128-Bit (may use linux tool uuidgen or random numbers via https://www.uuidgenerator.net/)
const String BEACON_UUID = "88973355-ff01-42b9-cdef-9d0ff2b3219d";

// define neighbor beacon
const String BEACON_NEI1 = "88973355-ff01-42b9-cfed-9d0ff2b3219d";
const String BEACON_NEI2 = "88973355-ff01-42b9-abcd-9d0ff2b3219d";

const char*ssid = "APM";
const char*password = "12345678gggg";
const String serverName = "https://breoteshr.ml";

// Root CA dari breoteshr.ml
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n"\
"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\n"\
"-----END CERTIFICATE-----\n";

/* Call Mode
0 = GET
1 = POST
2 = PUT
3 = Delete
*/

BLEAdvertising *pAdvertising;
static const char *eddystone_url_prefix_subs[] = {
  "http://www.",
  "https://www.",
  "http://",
  "https://",
  "urn:uuid:",
  NULL
};

static const char *eddystone_url_suffix_subs[] = {
  ".com/",
  ".org/",
  ".edu/",
  ".net/",
  ".info/",
  ".biz/",
  ".gov/",
  ".com",
  ".org",
  ".edu",
  ".net",
  ".info",
  ".biz",
  ".gov",
  NULL
};

//define feedback led
#define unregled 18
#define regled 19

//define Button
#define unregbtn 26
#define regbtn 27

//define Relays
#define relay1 25
#define relay2 33
#define relay3 32
#define relay4 35

//define relay state
#define RELAY_ON LOW
#define RELAY_OFF HIGH

//timer init
int waktu; 
unsigned long waktuSkrg, waktuLama;
int sampletime = 1000; // 1 detik

//sensor variable definition
int suhu;
int humid;
int ldr;

//SensorID array definition
String sensorid[2];

bool isRegistered = false;

// Not sure if WiFiClientSecure checks the validity date of the certificate. 
// Setting clock just to be sure...
void setClock() {
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print(F("Waiting for NTP time sync: "));
  time_t nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2) {
    delay(500);
    Serial.print(F("."));
    yield();
    nowSecs = time(nullptr);
  }

  Serial.println();
  struct tm timeinfo;
  gmtime_r(&nowSecs, &timeinfo);
  Serial.print(F("Current time: "));
  Serial.print(asctime(&timeinfo));
}


WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(9600);
  pinMode(regled, OUTPUT);
  pinMode(unregled, OUTPUT);
  pinMode(regbtn, INPUT_PULLUP);
  pinMode(unregbtn, INPUT_PULLUP);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  //turn off relay
  digitalWrite(relay1, RELAY_OFF);
  digitalWrite(relay2, RELAY_OFF);
  digitalWrite(relay3, RELAY_OFF);
  digitalWrite(relay4, RELAY_OFF);

  String url;
  // Create the BLE Device
  BLEDevice::init("MadeBeacon3");
  BLEDevice::setPower(ESP_PWR_LVL_P9);
  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  //config mode
  configLed();
  // wait for WiFi connection
  Serial.print("Waiting for WiFi to connect...");
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("  connected");
  setClock();
  finishStation();
}

void finishStation() {
  String url;
  do {
    setLed(isRegistered);  
    String webid = "beranda";
    url = RegisterStation(webid);
  
    if (url != "") {
      isRegistered = true;
      setLed(isRegistered);
      
      //Convert String to char array
      int str_len = url.length() + 1;  
      char url_char[str_len];
      url.toCharArray(url_char, str_len);
    
      //Mulai transmisi Physical Web
      //bluetooth publishing
      pAdvertising = BLEDevice::getAdvertising();
      setBeacon(url_char);
      pAdvertising->start();
    } else {
      Serial.println("Station registration failed!");
    }
  } while (url == "");
}

String RegisterStation(String webid) {
  DynamicJsonDocument doc(512);

  String reqdata = "{\"uuid\": \"";
  reqdata += BEACON_UUID;
  reqdata += "\", \"webid\": \"";
  reqdata += webid;
  reqdata +="\"}";
  Serial.println(reqdata);

  //Coba registrasi dulu lewat POST
  Serial.println("Register Station Begin...");
  String res = APICall(serverName, 1, "/", reqdata);
  Serial.println(res);
  //Jika gagal, coba cari entry dari station di server
  if (res == "") {
    //Mencari entry yang masih ada di server
    Serial.println("Register Station Failed, Begin fetching station data...");
    res = APICall(serverName, 0, ("/station/"+BEACON_UUID), "");
    Serial.println(res);
    if (res == "") return "";
  }
  deserializeJson(doc, res);
  JsonObject obj = doc.as<JsonObject>();
  String shortUrl = obj["data"]["shortUrl"];
  Serial.println(shortUrl);
  return shortUrl;
}


bool readSensor() {
  DynamicJsonDocument doc(512);
  StaticJsonDocument<200> filter;

  //set filter
  filter["data"]["sensor"][0]["value"] = true;
  filter["data"]["sensor"][0]["valueUnit"] = true;

  String out;
  serializeJson(doc, out);
  serializeJsonPretty(doc, Serial);
  Serial.println();
  String res;
  //Baca data sensor lewat GET
  Serial.println("Get Sensor Data Begin...");
  res = APICall(serverName, 0, "/sensor/" + BEACON_NEI1, out);

  deserializeJson(doc, res, DeserializationOption::Filter(filter));
  JsonObject obj = doc.as<JsonObject>();
  serializeJsonPretty(doc, Serial);
  Serial.println();

  String temp = obj["data"]["sensor"][0]["value"];
  humid = temp.toInt();
  String temp2 = obj["data"]["sensor"][1]["value"];
  suhu = temp2.toInt();

  //Baca data sensor lewat GET
  res = APICall(serverName, 0, "/sensor/" + BEACON_NEI2, out);

  deserializeJson(doc, res, DeserializationOption::Filter(filter));
  obj = doc.as<JsonObject>();
  serializeJsonPretty(doc, Serial);
  Serial.println();

  String temp3 = obj["data"]["sensor"][0]["value"];
  ldr = temp3.toInt();

  if (res == "") {
    return false;
  }
  return true;

}

bool UnregisterStation() {
  Serial.println("Unregister Station Begin...");
  String res = APICall(serverName, 3, ("/"+BEACON_UUID), "");
  Serial.println(res);
  res = APICall(serverName, 0, ("/"), "");
  Serial.println(res);
  pAdvertising->stop();
  isRegistered = false;
  
  if (res == "") {
   return false;
  }
  return true;

}

String GETAPIcall(WiFiClientSecure *client, String serverName, String path, String requestData) {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
      String payload;
      if (https.begin(*client, (serverName+path))) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload = https.getString();
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
            payload = https.getString();
        }
        https.end();
        return payload;
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
}

String POSTAPIcall(WiFiClientSecure *client, String serverName, String path, String requestData)  {
  // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
      String payload;
      if (https.begin(*client, (serverName+path))) {  // HTTPS
        Serial.print("[HTTPS] POST...\n");
        https.addHeader("Content-Type", "application/json");
        int httpCode = https.POST(requestData);
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload = https.getString();
          }
        } else {
          Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
        return payload;
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
} 

String DELETEAPIcall(WiFiClientSecure *client, String serverName, String path, String requestData)  {
  // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is 
      HTTPClient https;
      String payload;
      if (https.begin(*client, (serverName+path))) {  // HTTPS
        Serial.print("[HTTPS] DELETE...\n");
        int httpCode = https.sendRequest("DELETE");
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] DELETE... code: %d\n", httpCode);
  
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            payload = https.getString();
          }
        } else {
          Serial.printf("[HTTPS] DELETE... failed, error: %s\n", https.errorToString(httpCode).c_str());
            payload = https.getString();
        }
        https.end();
        return payload;
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
}


String APICall(String serverName, byte callMode, String path, String requestData) {
  WiFiClientSecure *client = new WiFiClientSecure;
  String payload;
  if(client) {
    client -> setCACert(rootCACertificate);
    {
      // Summon Switches
       switch(callMode) {
          case 0:
            // HTTPS GET
            payload = GETAPIcall(client, serverName, path, requestData);
            break;
          case 1:
            // POST Method
            payload = POSTAPIcall(client, serverName, path, requestData);
            break;
          case 2:
            // PUT Method
            break;
          case 3:
            // Delete Method
            payload = DELETEAPIcall(client, serverName, path, requestData);
            break;
          default:
            Serial.println("Unable to connect to HTTPS Server!");
        }
      // End extra scoping block
    }
    delete client;
  } else {
    Serial.println("Unable to create client");
  }
  return payload;
}

static int string_begin_with(const char *str, const char *prefix)
{
  int prefix_len = strlen(prefix);
  if (strncmp(prefix, str, prefix_len) == 0)
  {
    return prefix_len;
  }
  return 0;
}

void setBeacon(char* url)
{
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();

  int scheme_len, ext_len = 1, i, idx, url_idx;
  char *ret_data;
  int url_len = strlen(url);

  ret_data = (char *)calloc(1, url_len + 13);

  ret_data[0] = 2;   // Len
  ret_data[1] = 0x01;  // Type Flags
  ret_data[2] = 0x06;  // GENERAL_DISC_MODE 0x02 | BR_EDR_NOT_SUPPORTED 0x04
  ret_data[3] = 3;   // Len
  ret_data[4] = 0x03;  // Type 16-Bit UUID
  ret_data[5] = 0xAA;  // Eddystone UUID 2 -> 0xFEAA LSB
  ret_data[6] = 0xFE;  // Eddystone UUID 1 MSB
  ret_data[7] = 19; // Length of Beacon Data
  ret_data[8] = 0x16;  // Type Service Data
  ret_data[9] = 0xAA;  // Eddystone UUID 2 -> 0xFEAA LSB
  ret_data[10] = 0xFE; // Eddystone UUID 1 MSB
  ret_data[11] = 0x10; // Eddystone Frame Type
  ret_data[12] = 0xF4; // Beacons TX power at 0m

  i = 0, idx = 13, url_idx = 0;

  //replace prefix
  scheme_len = 0;
  while (eddystone_url_prefix_subs[i] != NULL)
  {
    if ((scheme_len = string_begin_with(url, eddystone_url_prefix_subs[i])) > 0)
    {
      ret_data[idx] = i;
      idx++;
      url_idx += scheme_len;
      break;
    }
    i++;
  }
  while (url_idx < url_len)
  {
    i = 0;
    ret_data[idx] = url[url_idx];
    ext_len = 1;
    while (eddystone_url_suffix_subs[i] != NULL)
    {
      if ((ext_len = string_begin_with(&url[url_idx], eddystone_url_suffix_subs[i])) > 0)
      {
        ret_data[idx] = i;
        break;
      }
      else
      {
        ext_len = 1; //inc 1
      }
      i++;
    }
    url_idx += ext_len;
    idx++;
  }
  ret_data[7] = idx - 8;

  Serial.printf("URL in data %s\n", &ret_data[13]);

  std::string eddyStoneData(ret_data);

  oAdvertisementData.addData(eddyStoneData);
  oScanResponseData.setName("MadeBeacon3");
  pAdvertising->setAdvertisementData(oAdvertisementData);
  pAdvertising->setScanResponseData(oScanResponseData);
}

void setLed(bool regstatus) {
  if (regstatus) {
    digitalWrite(regled, HIGH);
    digitalWrite(unregled, LOW);
  } else {
    digitalWrite(regled, LOW);
    digitalWrite(unregled, HIGH);
  }
}

void configLed() {
    digitalWrite(regled, HIGH);
    digitalWrite(unregled, HIGH);
}

void relayController(int setsuhu, int sethumid, int setldr) {
  if (suhu != 0 || humid != 0 || ldr != 0) {
    if (suhu <= setsuhu) {
      Serial.println("Relay1 ON");
      digitalWrite(relay1, RELAY_ON);
    }
    else {
      Serial.println("Relay1 OFF");
      digitalWrite(relay1, RELAY_OFF);
    }

    if (humid <= sethumid) {
      Serial.println("Relay2 ON");
      digitalWrite(relay2, RELAY_ON);
    }
    else {
      Serial.println("Relay2 OFF");
      digitalWrite(relay2, RELAY_OFF);
    }

    if (ldr <= setldr) {
      Serial.println("Relay3 ON");
      digitalWrite(relay3, RELAY_ON);
    }
    else {
      Serial.println("Relay3 OFF");
      digitalWrite(relay3, RELAY_OFF);
    }
  }
}

int cnt = 0;
void loop() {
  
  waktuSkrg = millis();
  waktu = ((int)(waktuSkrg - waktuLama));
  /*if (cnt == 30) {
    if (isRegistered) {
      UnregisterStation();
      setLed(isRegistered);
    }
    cnt = 0;
  }*/
    if (! digitalRead(unregbtn)) {
      Serial.println("TOMBOL1");
      configLed();
      delay(2000);
      if (isRegistered) {
        UnregisterStation();
      }
      setLed(isRegistered);
    } else if(! digitalRead(regbtn)) {
      Serial.println("TOMBOL2");
     configLed();
    delay(2000);
    if (!isRegistered) {
      finishStation();
    }
    setLed(isRegistered);
  }
  if ( waktu >= sampletime ) {
    if (isRegistered) {
      //GET Neighbor sensor value
      readSensor();
    }
    Serial.print("Salam P -> CNT: ");
    Serial.print(cnt);
    Serial.print(" Suhu: ");
    Serial.print(suhu);
    Serial.print(" Humid: ");
    Serial.print(humid);
    Serial.print(" LDR: ");
    Serial.println(ldr);
    cnt++;

    relayController(30, 85, 2000);
    
    waktuLama = waktuSkrg;
  }
}
