#include <M5Cardputer.h>
#include <M5Unified.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "font.h"

// Cardputer ADV IR Emitter Pin
const uint16_t kIrLed = 44;
IRsend irsend(kIrLed);

int selected = 0;      // Highlighted button index (0 to 14)
int brightnessLvl = 2; // Brightness mode: 2 = High (80), 1 = Dim (30), 0 = Low (5)
int helpPage = 0;      // 0 = Main Remote UI, 1 = Help Page 1, 2 = Help Page 2

struct SamsungBtn {
    const char* label;
    uint32_t code;
    char keyChar; // Direct key shortcut
};

// 15-Button Grid (5 Rows x 3 Columns)
const SamsungBtn samsungButtons[] = {
    // Row 1
    {"[P] PWR  ", 0xE0E040BF, 'p'},
    {"[-] VOL- ", 0xE0E0D02F, '-'},
    {"[=] VOL+ ", 0xE0E0E01F, '='},
    
    // Row 2
    {"[0] MUTE ", 0xE0E0F00F, '0'},
    {"[[] CH-  ", 0xE0E008F7, '['},
    {"[]] CH+  ", 0xE0E048B7, ']'},
    
    // Row 3
    {"[I] INPUT", 0xE0E0807F, 'i'},
    {"[;] UP   ", 0xE0E006F9, ';'},
    {"[\\] EXIT ", 0xE0E0B44B, '\\'},
    
    // Row 4
    {"[,] LEFT ", 0xE0E0A659, ','},
    {"[.] DOWN ", 0xE0E08679, '.'},
    {"[/] RIGHT", 0xE0E046B9, '/'},
    
    // Row 5
    {"[B] BACK ", 0xE0E01BA4, 'b'},
    {"[M] MENU ", 0xE0E058A7, 'm'},
    {"[SPC] OK ", 0xE0E016E9, ' '}
};

void drawHeader() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextColor(YELLOW, BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(5, 5);
    M5.Display.println("ADV Samsung Remote v.6");
}

void drawUI() {
    drawHeader();
    M5.Display.setTextSize(1);

    // Render 5x3 Grid
    for (int i = 0; i < 15; i++) {
        int x = 5 + (i % 3) * 78;
        int y = 26 + (i / 3) * 21;
        
        if (i == selected) {
            M5.Display.fillRect(x - 1, y - 1, 75, 18, WHITE);
            M5.Display.setTextColor(BLACK, WHITE);
        } else {
            M5.Display.drawRect(x - 1, y - 1, 75, 18, DARKGREY);
            M5.Display.setTextColor(CYAN, BLACK);
        }
        
        M5.Display.setCursor(x + 2, y + 4);
        M5.Display.print(samsungButtons[i].label);
    }
}

void drawHelpPage1() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextColor(YELLOW, BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(5, 5);
    M5.Display.println("--- KEY CONTROLS ---");

    M5.Display.setTextSize(1);
    M5.Display.setTextColor(CYAN, BLACK);
    M5.Display.setCursor(5, 30);
    M5.Display.println("[E,S,A,D] : Screen Cursor Nav");
    M5.Display.setCursor(5, 48);
    M5.Display.println("[ENTER/GO]: Send Highlighted");
    M5.Display.setCursor(5, 66);
    M5.Display.println("[Fn]      : Brightness (100/30/5%)");
    M5.Display.setCursor(5, 84);
    M5.Display.println("[P,-,=,etc]: Direct IR Trigger");

    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setCursor(5, 115);
    M5.Display.println("Press [H] -> Page 2   [ESC] -> Exit");
}

void drawHelpPage2() {
    M5.Display.fillScreen(BLACK);
    M5.Display.setTextColor(YELLOW, BLACK);
    M5.Display.setTextSize(2);
    M5.Display.setCursor(5, 5);
    M5.Display.println("--- ABOUT & CREDITS ---");

    M5.Display.setTextSize(1);
    M5.Display.setTextColor(GREEN, BLACK);
    M5.Display.setCursor(5, 30);
    M5.Display.println("Created by: izzthatright");
    M5.Display.setCursor(5, 45);
    M5.Display.println("            (with AI help)");

    M5.Display.setTextColor(LIGHTGREY, BLACK);
    M5.Display.setCursor(5, 68);
    M5.Display.println("Inspired by:");
    M5.Display.setCursor(15, 82);
    M5.Display.println("- VolosR (M5CardRemote)");
    M5.Display.setCursor(15, 96);
    M5.Display.println("- geo_tp (Ultimate Remote)");

    M5.Display.setTextColor(WHITE, BLACK);
    M5.Display.setCursor(5, 118);
    M5.Display.println("Press [H] or [ESC] -> Exit Help");
}

void setup() {
    auto cfg = M5.config();
    M5.begin(cfg);
    M5Cardputer.begin(cfg, true);

    M5.Display.setRotation(1);
    M5.Display.setBrightness(80);
    
    irsend.begin();

    drawUI();
}

void sendIR(uint32_t code) {
    M5.Display.fillRect(225, 5, 10, 10, RED);
    irsend.sendSAMSUNG(code, 32);
    delay(120);
    M5.Display.fillRect(225, 5, 10, 10, BLACK);
}

void toggleBrightness() {
    brightnessLvl = (brightnessLvl + 1) % 3;
    if (brightnessLvl == 2) M5.Display.setBrightness(80);      // High
    else if (brightnessLvl == 1) M5.Display.setBrightness(30); // Dim
    else M5.Display.setBrightness(5);                          // Stealth / Save Power
}

void loop() {
    M5.update();
    M5Cardputer.update();

    if (M5Cardputer.Keyboard.isPressed()) {
        auto status = M5Cardputer.Keyboard.keysState();
        bool needsRedraw = false;

        // 1. Check Help Toggle Key ('H')
        if (M5Cardputer.Keyboard.isKeyPressed('h') || M5Cardputer.Keyboard.isKeyPressed('H')) {
            helpPage = (helpPage + 1) % 3;
            if (helpPage == 1) drawHelpPage1();
            else if (helpPage == 2) drawHelpPage2();
            else drawUI();
            delay(200); // Debounce
            return;
        }

        // If in Help mode, ESC or Space/Enter closes Help and returns to Main UI
        if (helpPage > 0) {
            if (status.del || M5Cardputer.Keyboard.isKeyPressed('\n') || M5Cardputer.Keyboard.isKeyPressed(' ')) {
                helpPage = 0;
                drawUI();
                delay(200);
            }
            return; // Block IR transmission while reading help
        }

        // 2. Check Fn Key (Display Dimming)
        if (status.fn) {
            toggleBrightness();
            delay(200);
            return;
        }

        // 3. Direct Key Pressing (Instant IR Fire)
        for (char c : status.word) {
            char lowerC = tolower(c);
            for (int i = 0; i < 15; i++) {
                if (lowerC == samsungButtons[i].keyChar) {
                    selected = i;
                    drawUI();
                    sendIR(samsungButtons[i].code);
                    delay(150);
                    return;
                }
            }
        }

        // 4. Screen Navigation Keys (E, S, A, D)
        if (M5Cardputer.Keyboard.isKeyPressed('e') || M5Cardputer.Keyboard.isKeyPressed('E')) {
            if (selected >= 3) selected -= 3;
            needsRedraw = true;
        }
        if (M5Cardputer.Keyboard.isKeyPressed('s') || M5Cardputer.Keyboard.isKeyPressed('S')) {
            if (selected <= 11) selected += 3;
            needsRedraw = true;
        }
        if (M5Cardputer.Keyboard.isKeyPressed('a') || M5Cardputer.Keyboard.isKeyPressed('A')) {
            if (selected % 3 > 0) selected -= 1;
            needsRedraw = true;
        }
        if (M5Cardputer.Keyboard.isKeyPressed('d') || M5Cardputer.Keyboard.isKeyPressed('D')) {
            if (selected % 3 < 2) selected += 1;
            needsRedraw = true;
        }

        // 5. Enter Key -> Transmit highlighted button
        if (M5Cardputer.Keyboard.isKeyPressed('\n')) {
            sendIR(samsungButtons[selected].code);
            delay(150);
            return;
        }

        if (needsRedraw) {
            drawUI();
            delay(120);
        }
    }

    // 6. Hardware GO Button (BtnA)
    if (M5.BtnA.wasPressed()) {
        if (helpPage > 0) {
            helpPage = 0;
            drawUI();
        } else {
            sendIR(samsungButtons[selected].code);
        }
    }

    delay(20);
}