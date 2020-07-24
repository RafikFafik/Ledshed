#include <ArduinoOTA.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define LED_COUNT 485
// #define LED_COUNT 106
#define SWITCH_PIN 0
#define DEBUG_LED 4
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);
int light = 50;
const char* ssid = "SpaceX";
const char* password = "";
const bool PROGMEM disabled[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };

int currentLed = 0;

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


const char webhtml[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="pl">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
    <title>Ledshed</title>
    <style>
        body {
            background-color: #3b0e19;
            font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
            color: white;
        }

        .form {
            display: grid;
            grid-template-rows: repeat(5, 1fr);
            grid-template-columns: 1fr 1fr;
            grid-column: 1 / 3;
        }

        input[type=range] {
            display: flex;
            margin: auto;
            padding: 20px;
            font-size: 3rem;
            justify-content: center;
        }
        input[type=button] {
            padding: 20px;
        }
        .warmwhite {
            background-color: rgb(255, 211, 89);
        }

        .response {
            grid-column: 1 / 3;
            align-self: center;
            margin-left: auto;
            margin-right: auto;
        }

        p {
            color: white;
        }

        @media only screen and (max-width : 1440px) and (max-height : 2560px) {
            body {
                background-color: #3b0e19;
                font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif
            }
            [name=container] {
                display: grid;
                grid-template-rows: repeat(1fr, 4);
                grid-template-columns: 50% 50%;
            }
            input[type=range] {
                
                grid-column: span 2;
                width: 60vw;
                
            }
            input[type=button] {
                /* grid-column: span 2; */
                width: 20vw;

            }
            label {
                margin:auto;
                grid-column: span 2;
            }
            [name=eachled-buttons] {
                display: grid;
                grid-template-columns: repeat(1fr, 3);
                grid-column: span 2;
                display: block;
            }
        }
    </style>
</head>

<body>
    <div name="container">
        <label for="luminosity">Jasność</label>
        <input name="luminosity" type="range" min="15" max="255" value="15" step="10">
        <label for="r">RGB</label>
        <input name="r" type="range" min="0" max="255" value="0" step="10">
        <input name="g" type="range" min="0" max="255" value="0" step="10">
        <input name="b" type="range" min="0" max="255" value="0" step="10">
        <label for="eachled">Punkt</label> 
        <input name="eachled" type="range" min="0" max="484" value="0">
        <div name="eachled-buttons">
            <input name="minus" type="button" value="-">
            <input name="plus" type="button" value="+">
            <input name="val" type="text" disabled value="0">
        </div>
        <label for="mod">Modulo</label> 
        <input name="mod" type="range" min="0" max="105" value="0">
        <div name="mod-buttons">
            <input name="mod-minus" type="button" value="-">
            <input name="mod-plus" type="button" value="+">
            <input name="mod-val" type="text" disabled value="0">
        </div>
        <div name="ran-buttons">
            <input name="ran-minus" type="button" value="r-">
            <input name="ran-plus" type="button" value="r+">
            <input name="ran-val" type="text" disabled value="0">
        </div>
        <input name="rainbow" type='button' value='&#127752'>
        <input name="balls" type='button' value='&#9917'>
        <input name="warmwhite" value="On" onclick="turnOn()" type='button'>
        <input name="off" type='button' value='Off' >
    </div>
    <script>
        const url = "http://192.168.69.215";

        var luminosity = document.querySelector('input');
        var r = document.querySelector('[name=r]');
        var g = document.querySelector('[name=g]');
        var b = document.querySelector('[name=b]');
        var rainbow = document.querySelector('[name=rainbow]');
        var balls = document.querySelector('[name=balls]');
        var eachled = document.querySelector('[name=eachled]');
        var plus = document.querySelector('[name=plus]');
        var minus = document.querySelector('[name=minus]');
        var val = document.querySelector('[name=val]');
        var off = document.querySelector('[name=off]');
        var mod = document.querySelector('[name=mod]');
        var mod_plus = document.querySelector('[name=mod-plus]');
        var mod_minus = document.querySelector('[name=mod-minus]');
        var mod_val = document.querySelector('[name=mod-val]');
        var ran_plus = document.querySelector('[name=ran-plus]');
        var ran_minus = document.querySelector('[name=ran-minus]');
        var ran_val = document.querySelector('[name=ran-val]');

        var request = new XMLHttpRequest();

        off.onclick = function() {
            sendRequest('POST', '/off');
        }

        ran_plus.onclick = function() {
            ran_val.value = Number(ran_val.value) + 1;
            sendRequest('POST', '/mod', 'mod=' + mod.value + '&range=' + ran_val.value);
        }
        ran_minus.onclick = function() {
            ran_val.value = ran_val.value - 1;
            sendRequest('POST', '/mod', 'mod=' + mod.value + '&range=' + ran_val.value);
        }

        mod_plus.onclick = function() {
            console.log(mod.value);
            mod.value = Number(mod.value) + 1;
            mod_val.value = mod.value;
            sendRequest('POST', '/mod', 'mod=' + mod.value + '&range=' + ran_val.value);
        }
        mod_minus.onclick = function() {
            mod.value -= 1;
            mod_val.value = mod.value;
            sendRequest('POST', '/mod', 'mod=' + mod.value + '&range=' + ran_val.value);
        }

        mod.oninput = function() {
            mod_val.value = this.value;
            sendRequest('POST', '/mod', 'mod=' + mod.value + '&range=' + ran_val.value);
        }

        plus.onclick = function() {
            console.log(eachled.value);
            eachled.value = Number(eachled.value) + 1;
            val.value = eachled.value;
            sendRequest('POST', '/point', 'point=' + eachled.value);
        }
        minus.onclick = function() {
            eachled.value -= 1;
            val.value = eachled.value;
            sendRequest('POST', '/point', 'point=' + eachled.value);
        }

        eachled.oninput = function() {
            val.value = this.value;
            sendRequest('POST', '/point', 'point=' + eachled.value);
        }

        balls.onclick = function() {
            sendRequest('GET', '/balls');
        }

        rainbow.onclick = function() {
            sendRequest('POST', '/rainbow');
        }

        luminosity.oninput = function() {
            sendRequest('POST', '/luminosity', 'value=' + this.value);
        };

        r.oninput = function() {
            var params = 'r=' + r.value + '&g=' + g.value + '&b=' + b.value;
            sendRequest('POST', '/rgb', params);
        }
        g.oninput = function() {
            var params = 'r=' + r.value + '&g=' + g.value + '&b=' + b.value;
            sendRequest('POST', '/rgb', params);
        }
        b.oninput = function() {
            var params = 'r=' + r.value + '&g=' + g.value + '&b=' + b.value;
            sendRequest('POST', '/rgb', params);
        }

        function turnOn() {
            request.open("GET", url + "/warmwhite", true);
            request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
            request.onreadystatechange = function() {
                if(this.readyState == 4 && this.status == 200)
                    console.log(this.response);
            }
            request.send();
        }

        function sendRequest(method, view, params) {
            request.open(method, url + view, true);
            request.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
            request.onreadystatechange = function() {
                if(this.readyState == 4 && this.status == 200)
                    console.log(this.response);
            }
            request.send(params);
        }
    </script>
</body>

</html>
)=====";

int mTime = 0;
bool lastState = 0;
bool isLighting = false;
bool flag = true;

ICACHE_RAM_ATTR void manualSwitch() {
  if(millis() - mTime > 500 ) {
    lastState = !lastState;
    mTime = millis();
    isLighting ? handleOff() : handleWarmWhite();
  }
}


void setup(void){
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), manualSwitch, CHANGE);
 
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_LED, OUTPUT);
  pinMode(16, OUTPUT);
  strip.begin();
  strip.setBrightness(50);
  Serial.begin(9600);
  delay(10);
  Serial.println('\n');
  wifiMulti.addAP(ssid, password);
  Serial.println("Connecting ...");

  while (wifiMulti.run() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("ledy")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  
  server.on("/", handleRoot);  
  server.on("/rgb", handleRGB);
  server.on("/off", handleOff);
  server.on("/rainbow", handleRainbow);
  server.on("/luminosity", handleLuminosity);
  server.on("/warmwhite", handleWarmWhite);
  server.on("/balls", handleBalls);
  server.on("/point", handlePoint);
  server.on("/mod", handleMod);
  server.on("/restart", handleRestart);
  server.onNotFound(handleNotFound);
  server.begin();
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  rebootFinished();
}


void loop(void){
  server.handleClient();   
  ArduinoOTA.handle();
}

void handleRestart() {
  ESP.restart();
}
void handleRoot() {
  server.send(200, "text/html", webhtml);
}
void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void setVisiblePixelColor(int index, int r, int g, int b) {
  if(!pgm_read_byte(&disabled[index]))
    strip.setPixelColor(index, pgm_read_byte(&gamma8[r]), pgm_read_byte(&gamma8[g]), pgm_read_byte(&gamma8[b]));
  else
    strip.setPixelColor(index, 0, 0, 0);
}

void handlePoint() {
  flag = false;
  long point = server.arg("point").toInt();
  server.send(200, "text/plain", "Point!");
  strip.clear();
  strip.setPixelColor(point, 255, 255, 255);
  strip.show();
  digitalWrite(4, HIGH);
}

void handleMod() {
  flag = false;
  long mod = server.arg("mod").toInt();
  long range = server.arg("range").toInt();
  server.send(200, "text/plain", "Point!");
  strip.clear();
  for(int i = range; i < strip.numPixels() - range; i++) {
    if(i % mod < range)
      strip.setPixelColor(i - range, 255, 0, 0);
    else
      strip.setPixelColor(i - range, 0, 255, 0);
  } 
  strip.show();
  digitalWrite(4, HIGH);
}

void handleRGB() {
  flag = false;
  long r = server.arg("r").toInt();
  long g = server.arg("g").toInt();
  long b = server.arg("b").toInt();
  strip.setBrightness(light);
  for(int i = 0; i < strip.numPixels(); i++) {
    setVisiblePixelColor(i, r, g, b);
  } 
  strip.show();
  digitalWrite(4, HIGH);
}

void handleOff() {
  flag = false;
  server.sendHeader("Location", "/");
  server.send(301, "text/plain", "Leds turned off!");
  strip.setBrightness(0);
  strip.show();
  digitalWrite(4, LOW);
  isLighting = false;
}
void handleRainbow() {
  isLighting = true;
  server.send(200, "text/plain", "Turn On!"); 
  flag = true;
  strip.setBrightness(light);
  rainbow();
}
void handleWarmWhite() {
  isLighting = true;
  flag = false;
  server.send(200, "text/plain", "Turn On!"); 
  strip.setBrightness(light);
  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 255, 150, 100);
  }
  strip.show();
}
void handleBalls() {
  isLighting = true;
  flag = false;
  server.sendHeader("Location", "/");
  server.send(200, "text/html", "Cold white mode"); 
  flag = true;
  strip.setBrightness(light);
  BouncingBalls(0xd4,0xaf,0x37, 2);
}

void handleLuminosity() {
  server.sendHeader("Location", "/");
  long value;
  if(server.hasArg("value")) {
    value = server.arg("value").toInt();
    light = value;
  }
  server.send(200, "text/plain", String(light));
  strip.setBrightness(light);
  strip.show();
}

void rainbow() {
  uint16_t i, j = 0;
  
  while(flag) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i*1+j) & 255));
    }
    server.handleClient();  
    strip.show();
    j++;
    if(j > 255)
      j = 0;
  }
  
  if(flag)
    rainbow();
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// bouncingBalls

void BouncingBalls(byte red, byte green, byte blue, int BallCount) {
  float Gravity = -1.81;
  int StartHeight = 1;
 
  float Height[BallCount];
  float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int   Position[BallCount];
  long  ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];
 
  for (int i = 0 ; i < BallCount ; i++) {  
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
  }

  while (flag) {
    for (int i = 0 ; i < BallCount ; i++) {
      TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
      Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;
 
      if ( Height[i] < 0 ) {                      
        Height[i] = 0;
        ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
        ClockTimeSinceLastBounce[i] = millis();
 
        if ( ImpactVelocity[i] < 0.01 ) {
          ImpactVelocity[i] = ImpactVelocityStart;
        }
      }
      Position[i] = round( Height[i] * (strip.numPixels() / 2) / StartHeight);
    }
 
    for (int i = 0 ; i < BallCount ; i++) {
      strip.setPixelColor(Position[i],red,green,blue);
    }
    server.handleClient();  
    strip.show();
    strip.clear();
  }
}

void rebootFinished() {
  strip.clear();
  strip.fill(155155245);
  strip.show();
  digitalWrite(DEBUG_LED, HIGH);
  delay(100);
  strip.fill();
  strip.show();
  digitalWrite(DEBUG_LED, LOW);
  delay(100);
  strip.fill(155155245);
  strip.show();
  digitalWrite(DEBUG_LED, HIGH);
  delay(100);
  strip.fill();
  strip.show();
  digitalWrite(DEBUG_LED, LOW);
}
