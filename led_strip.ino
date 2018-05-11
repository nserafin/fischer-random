/*
 * Copyright 2018, Alyssa Casamento, Josh LeFevre, Nathan Serafin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <Adafruit_NeoPixel.h>

constexpr int TDATA_PIN = 2;
constexpr int BDATA_PIN = 3;
constexpr int BUTTON_PIN = 4;
constexpr int SRAND_PIN = A0;

constexpr uint32_t
color(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint32_t) r) << 16) | (((uint32_t) g) << 8) | ((uint32_t) b);
}

constexpr uint32_t OFF_COLOR = color(0, 0, 0);
constexpr uint32_t READY_COLOR = color(0, 255, 0);
constexpr uint32_t ROOK_COLOR = color(255, 0, 0);
constexpr uint32_t KNIGHT_COLOR = color(0, 255, 0);
constexpr uint32_t BISHOP_COLOR = color(0, 0, 255);
constexpr uint32_t KING_COLOR = color(255, 255, 255);
constexpr uint32_t QUEEN_COLOR = color(255, 0, 255);

constexpr int NPIXELS = 24;
constexpr int ROW_SIZE = 8;

uint32_t READY_DISPLAY[] = {READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR,
                            READY_COLOR, OFF_COLOR, OFF_COLOR};

typedef enum {
    EMPTY = 0,
    KING = 1,
    QUEEN = 2,
    ROOK = 3,
    BISHOP = 4,
    KNIGHT = 5,
} piece_t;

bool prev_button = 0;
bool go = false;
bool pattern = false;

uint32_t tdisplay[NPIXELS];
uint32_t bdisplay[NPIXELS];

Adafruit_NeoPixel tstrip = Adafruit_NeoPixel(NPIXELS, TDATA_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bstrip = Adafruit_NeoPixel(NPIXELS, BDATA_PIN, NEO_GRB + NEO_KHZ800);

static void
display_setup(Adafruit_NeoPixel& strip, uint32_t c[])
{
    strip.setPixelColor(0,  c[0]);
    strip.setPixelColor(1,  c[1]);
    strip.setPixelColor(2,  c[2]);
    strip.setPixelColor(3,  c[3]);
    strip.setPixelColor(4,  c[4]);
    strip.setPixelColor(5,  c[5]);
    strip.setPixelColor(6,  c[6]);
    strip.setPixelColor(7,  c[7]);
    strip.setPixelColor(8,  c[8]);
    strip.setPixelColor(9,  c[9]);
    strip.setPixelColor(10, c[10]);
    strip.setPixelColor(11, c[11]);
    strip.setPixelColor(12, c[12]);
    strip.setPixelColor(13, c[13]);
    strip.setPixelColor(14, c[14]);
    strip.setPixelColor(15, c[15]);
    strip.setPixelColor(16, c[16]);
    strip.setPixelColor(17, c[17]);
    strip.setPixelColor(18, c[18]);
    strip.setPixelColor(19, c[19]);
    strip.setPixelColor(20, c[20]);
    strip.setPixelColor(21, c[21]);
    strip.setPixelColor(22, c[22]);
    strip.setPixelColor(23, c[23]);

    strip.show();
}

static void
display_back_row(Adafruit_NeoPixel& tstrip, Adafruit_NeoPixel& bstrip,
                 piece_t back_row[])
{

    for (int i = 0; i < ROW_SIZE; i++) {
        switch (back_row[i]) {
            case KING:
                tdisplay[i * 3]     = KING_COLOR;
                tdisplay[i * 3 + 1] = OFF_COLOR;
                tdisplay[i * 3 + 2] = OFF_COLOR;
                bdisplay[i * 3]     = OFF_COLOR;
                bdisplay[i * 3 + 1] = OFF_COLOR;
                bdisplay[i * 3 + 2] = OFF_COLOR;
                break;
            case QUEEN:
                tdisplay[i * 3]     = OFF_COLOR;
                tdisplay[i * 3 + 1] = QUEEN_COLOR;
                tdisplay[i * 3 + 2] = OFF_COLOR;
                bdisplay[i * 3]     = OFF_COLOR;
                bdisplay[i * 3 + 1] = OFF_COLOR;
                bdisplay[i * 3 + 2] = OFF_COLOR;
                break;
            case BISHOP:
                tdisplay[i * 3]     = OFF_COLOR;
                tdisplay[i * 3 + 1] = OFF_COLOR;
                tdisplay[i * 3 + 2] = BISHOP_COLOR;
                bdisplay[i * 3]     = OFF_COLOR;
                bdisplay[i * 3 + 1] = OFF_COLOR;
                bdisplay[i * 3 + 2] = OFF_COLOR;
                break;
            case KNIGHT:
                tdisplay[i * 3]     = OFF_COLOR;
                tdisplay[i * 3 + 1] = OFF_COLOR;
                tdisplay[i * 3 + 2] = OFF_COLOR;
                bdisplay[i * 3]     = KNIGHT_COLOR;
                bdisplay[i * 3 + 1] = OFF_COLOR;
                bdisplay[i * 3 + 2] = OFF_COLOR;
                break;
            case ROOK:
                tdisplay[i * 3]     = OFF_COLOR;
                tdisplay[i * 3 + 1] = OFF_COLOR;
                tdisplay[i * 3 + 2] = OFF_COLOR;
                bdisplay[i * 3]     = OFF_COLOR;
                bdisplay[i * 3 + 1] = ROOK_COLOR;
                bdisplay[i * 3 + 2] = OFF_COLOR;
                break;
        }
    }

    display_setup(tstrip, tdisplay);
    display_setup(bstrip, bdisplay);
}

void
setup()
{
    pinMode(TDATA_PIN, OUTPUT);
    pinMode(BDATA_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(SRAND_PIN, INPUT);

    randomSeed(analogRead(SRAND_PIN));

    tstrip.begin();
    bstrip.begin();

    tstrip.setBrightness(64);
    bstrip.setBrightness(64);

    tstrip.show();
    bstrip.show();
}

void
loop()
{
    bool button = !digitalRead(BUTTON_PIN);
    if (button != prev_button) {
        delay(100);
        prev_button = button;
        if (button) {
            go = true;
            pattern = true;
        }
    }

    if (!go) {
        display_setup(tstrip, READY_DISPLAY);
        display_setup(bstrip, READY_DISPLAY);
    }
    else if (pattern) {
        piece_t back_row[8] = {EMPTY, EMPTY, EMPTY, EMPTY,
                               EMPTY, EMPTY, EMPTY, EMPTY};
        int K, Q, R1, R2, BI1, BI2;

        int color1_cnt = 4; // Even.
        int color2_cnt = 4; // Odd.

        /* Place king. */
        if (((K = random(1, 6)) % 2) == 0) {
            color1_cnt--;
        }
        else {
            color2_cnt--;
        }
        /* Place first rook. */
        if (((R1 = random(0, K - 1)) % 2) == 0) {
            color1_cnt--;
        }
        else {
            color2_cnt--;
        }
        /* Place second rook. */
        if (((R2 = random(K + 1, 7)) % 2) == 0) {
            color1_cnt--;
        }
        else {
            color2_cnt--;
        }

        back_row[K] = KING;
        back_row[R1] = ROOK;
        back_row[R2] = ROOK;

        /* Place first bishop. */
        BI1 = random(0, color1_cnt - 1);
        int n = 0;
        for (int i = 0; i < ROW_SIZE; i += 2) {
            if (back_row[i] == EMPTY) {
                if (BI1 == n) {
                    back_row[i] = BISHOP;
                    break;
                }
                n++;
            }
        }

        /* Place second bishop. */
        BI2 = random(0, color2_cnt - 1);
        n = 0;
        for (int i = 1; i < ROW_SIZE; i += 2) {
            if (back_row[i] == EMPTY) {
                if (BI2 == n) {
                    back_row[i] = BISHOP;
                    break;
                }
                n++;
            }
        }

        /* Place queen. */
        Q = random(0, 3);
        n = 0;
        for (int i = 0; i < ROW_SIZE; i++) {
            if (back_row[i] == EMPTY) {
                if (Q == n) {
                    back_row[i] = QUEEN;
                    break;
                }
                n++;
            }
        }

        /* Place knights. */
        for (int i = 0; i < ROW_SIZE; i++) {
            if (back_row[i] == EMPTY) {
                back_row[i] = KNIGHT;
            }
        }

        display_back_row(tstrip, bstrip, back_row);
        pattern = false;
    }
}
