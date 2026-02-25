#pragma once

int applyDeadzone(int value, int deadzone) {
  
  
    if (abs(value) < deadzone) {
    return 0;
  } else {
    // Skaliere den Wert jenseits der Deadzone
    if (value > 0) {
      return map(value, deadzone, 511, 1, 511);
    } else {
      return map(value, -511, -deadzone, -511, -1);
    }
  }
}