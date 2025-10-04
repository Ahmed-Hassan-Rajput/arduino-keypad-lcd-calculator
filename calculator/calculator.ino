#include <LiquidCrystal.h>
#include <Keypad.h>

// LCD: rs=0, en=1, d4=2, d5=3, d6=4, d7=5  
LiquidCrystal lcd(0, 1, 2, 3, 4, 5);

// Keypad 4×4
const byte ROW = 4, COLM = 4;

char values[ROW][COLM] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},
  {'C', '0', '=', '/'}
};

byte pinsROW[ROW]   = {13, 12, 11, 10};
byte pinsCOLM[COLM] = {9, 8, 7, 6};

Keypad myKeypad = Keypad(makeKeymap(values), pinsROW, pinsCOLM, ROW, COLM);

// --- Calculator state ---
bool   currentNum = false;  // false: entering first number; true: entering second
bool   finalNum   = false;  // true after at least one digit of num2
String num1, num2;
char   op = 0;              // '+','-','*','/'; 0 means none yet
float  answer = 0.0;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Keypad Calculator");
  lcd.setCursor(0, 1);
  lcd.print("Ready");
  delay(800);
  lcd.clear();
}

void loop() {
  char key = myKeypad.getKey();
  if (key == NO_KEY) return;

  // --- Digits ---
  if (key >= '0' && key <= '9') {
    if (!currentNum) {
      // entering first number (top row)
      num1 += key;
      int n = num1.length();
      lcd.setCursor(15 - n, 0);  // right-align
      lcd.print(num1);
    } else {
      // entering second number (bottom row)
      num2 += key;
      int n = num2.length();
      lcd.setCursor(15 - n, 1);  // right-align
      lcd.print(num2);
      finalNum = true;
    }
    return;
  }

  // --- Operators ---
  if (!currentNum && (key == '+' || key == '-' || key == '*' || key == '/')) {
    op = key;
    currentNum = true;
    lcd.setCursor(15, 0);  // show operator at far right of top line
    lcd.print(op);
    return;
  }

  // --- Equals: compute ---
  if (finalNum && key == '=') {
    float value1 = num1.toFloat();
    float value2 = num2.toFloat();

    if      (op == '+') answer = value1 + value2;
    else if (op == '-') answer = value1 - value2;
    else if (op == '*') answer = value1 * value2;
    else if (op == '/') answer = (value2 != 0.0f) ? (value1 / value2) : 0.0f;

    lcd.clear();
    lcd.setCursor(15, 0);
    lcd.autoscroll();
    lcd.print(answer, 2);   // print with 2 decimal places
    lcd.noAutoscroll();

    // reset for next calculation
    num1 = "";
    num2 = "";
    op = 0;
    answer = 0.0f;
    currentNum = false;
    finalNum = false;
    return;
  }

  // --- Clear ---
  if (key == 'C') {
    lcd.clear();
    num1 = "";
    num2 = "";
    op = 0;
    answer = 0.0f;
    currentNum = false;
    finalNum = false;
    return;
  }

  
}
