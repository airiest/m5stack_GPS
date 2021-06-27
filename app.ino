#include <M5Stack.h>
#include <time.h>
#include <BluetoothSerial.h>
#include <TinyGPS++.h>
#include "ScrollTextWindow.h"

#define TOP_FIXED_HEIGHT 0    // 上部の固定エリア高さ
#define BOTTOM_FIXED_HEIGHT 0 // 下部の固定エリア高さ

typedef enum
{
    MODE_ENCODE,
    MODE_NMEA
} MODE;

BluetoothSerial SerialBT;
TinyGPSPlus gps;
ScrollTextWindow *stw;
MODE mode = MODE_ENCODE;
bool scroll = true;

void setup()
{
    M5.begin();
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(2);

#if 0
    // 固定エリアの描画
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLUE);

    // 上部固定エリア
    M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, TOP_FIXED_HEIGHT, TFT_BLUE);
    M5.Lcd.drawString("TOP FIXED AREA", SCREEN_WIDTH / 2, TOP_FIXED_HEIGHT / 2);

    // 下部固定エリア
    M5.Lcd.fillRect(0, SCREEN_HEIGHT - BOTTOM_FIXED_HEIGHT, SCREEN_WIDTH, BOTTOM_FIXED_HEIGHT, TFT_BLUE);
    M5.Lcd.drawString("BOTTOM FIXED AREA", SCREEN_WIDTH / 2, SCREEN_HEIGHT - BOTTOM_FIXED_HEIGHT / 2);
#endif

    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

    stw = new ScrollTextWindow(TOP_FIXED_HEIGHT, BOTTOM_FIXED_HEIGHT, TFT_BLACK, 12, 30);

    //Serial.begin(115200);
    SerialBT.begin("M5Stack");
    Serial2.begin(9600, SERIAL_8N1, 22, 21);
}

void loop(void)
{
    char msg[256] = "";
    time_t utc;
    tm ymdhms;

    M5.update();
    if (M5.BtnA.wasPressed())
    {
        mode = MODE_ENCODE;
        M5.Lcd.clear();
        stw->cls();
    }
    else if (M5.BtnB.wasPressed())
    {
        mode = MODE_NMEA;
        scroll = true;
        M5.Lcd.clear();
        stw->cls();
    }
    else if (M5.BtnC.wasPressed())
    {
        scroll = scroll ? false : true;
    }

    while (Serial2.available() > 0)
    {
        char data = Serial2.read();
        gps.encode(data);
        if (mode == MODE_NMEA)
        {
            if (scroll)
            {
                stw->print(data);
            }
        }

        if (gps.time.isUpdated())
        {
            ymdhms.tm_year = gps.date.year() - 1900;
            ymdhms.tm_mon = gps.date.month();
            ymdhms.tm_mday = gps.date.day();
            ymdhms.tm_hour = gps.time.hour();
            ymdhms.tm_min = gps.time.minute();
            ymdhms.tm_sec = gps.time.second();
            utc = mktime(&ymdhms);

            if (mode == MODE_ENCODE)
            {
                M5.Lcd.setCursor(0, 0);
                M5.Lcd.printf("%04d/%02d/%02d %02d:%02d:%02d", ymdhms.tm_year + 1900, ymdhms.tm_mon, ymdhms.tm_mday, ymdhms.tm_hour, ymdhms.tm_min, ymdhms.tm_sec);
            }
            sprintf(msg, "time,%d\n", utc);
            SerialBT.print(msg);
        }

        if (gps.location.isUpdated())
        {
            if (mode == MODE_ENCODE)
            {
                M5.Lcd.setCursor(0, 30);
                M5.Lcd.printf("lat      : %.6f", gps.location.lat());
                M5.Lcd.setCursor(0, 60);
                M5.Lcd.printf("lng      : %.6f", gps.location.lng());
            }
            sprintf(msg, "latlng,%d,%.6f,%.6f\n", utc, gps.location.lat(), gps.location.lng());
            SerialBT.print(msg);
        }

        if (gps.altitude.isUpdated())
        {
            if (mode == MODE_ENCODE)
            {
                M5.Lcd.setCursor(0, 90);
                M5.Lcd.printf("altitude : %.1f", gps.altitude.meters());
            }
            sprintf(msg, "altitude,%d,%.1f\n", utc, gps.altitude.meters());
            SerialBT.print(msg);
        }

        if (gps.speed.isUpdated())
        {
            if (mode == MODE_ENCODE)
            {
                M5.Lcd.setCursor(0, 120);
                M5.Lcd.printf("speed    : %.1f", gps.speed.mph());
            }
            sprintf(msg, "speed,%d,%.1f\n", utc, gps.speed.mph());
            SerialBT.print(msg);
        }

        if (gps.course.isUpdated())
        {
            if (mode == MODE_ENCODE)
            {
                M5.Lcd.setCursor(0, 150);
                M5.Lcd.printf("course   : %.1f", gps.course.deg());
            }
            sprintf(msg, "cource,%d,%.1f\n", utc, gps.course.deg());
            SerialBT.print(msg);
        }
    }
}
