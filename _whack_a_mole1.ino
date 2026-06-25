// =====================================================
// WHACK A MOLE - ARDUINO UNO
// Hardware:
//  - LEDs verdes: 9, 8, 7
//  - Botones:     10, 11, 12
//  - LEDs rojos:  6, 5, 4, 3, 2
//
// Reglas:
//  - Botón 10 = Fácil  -> 1 LED verde encendido
//  - Botón 11 = Medio  -> 2 LEDs verdes encendidos
//  - Botón 12 = Difícil-> 3 LEDs verdes encendidos
//  - Luego cualquier botón empieza la partida
//  - Durante el juego: SOLO 1 topo a la vez
//  - Si el topo es trampa, los rojos parpadean para avisar
//  - Si golpeas la trampa, pierdes la mitad de tus puntos
//  - Botón incorrecto = -1 punto
//  - Al terminar: botones bloqueados
//  - Los rojos suben uno por uno
//  - Si score > 50: la máquina se vuelve loca
// =====================================================

// ----------------------
// PINES
// ----------------------
const byte greenPins[3]  = {9, 8, 7};
const byte buttonPins[3] = {10, 11, 12};
const byte redPins[5]    = {6, 5, 4, 3, 2};

// ----------------------
// ESTADOS
// ----------------------
enum GameState {
  SELECT_DIFFICULTY,
  WAIT_START,
  PLAYING,
  FINAL_REVEAL,
  FINAL_HOLD
};

GameState state = SELECT_DIFFICULTY;

// ----------------------
// TIEMPOS
// ----------------------
const unsigned long GAME_TIME_MS          = 30000UL; // 30 segundos
const unsigned long REVEAL_STEP_MS        = 400UL;   // LEDs rojos uno por uno
const unsigned long FINAL_RESTART_LOCK_MS  = 2500UL;  // bloqueo final antes de reiniciar
const unsigned long TRAP_BLINK_MS         = 100UL;   // parpadeo de trampa

// ----------------------
// DIFICULTAD
// ----------------------
byte difficulty = 1;                 // 1=fácil, 2=medio, 3=difícil
unsigned long reactionTimeMs = 1200; // cambia según dificultad
byte trapChance = 10;                // % de probabilidad de trampa

// ----------------------
// JUEGO
// ----------------------
int score = 0;

unsigned long gameStartMs = 0;
unsigned long roundStartMs = 0;

byte activeTopo = 255;     // 0,1,2 = pin verde activo
bool trapActive = false;

byte previousTopo = 255;

// ----------------------
// FINAL
// ----------------------
byte finalRevealCount = 0;
byte finalTargetReds = 0;
byte finalMode = 1; // 1 = normal, 2 = parpadeo loco, 3 = caos total
unsigned long finalStepMs = 0;
unsigned long finalEffectMs = 0;
unsigned long finalHoldStartMs = 0;
bool finalBlinkState = false;

// ----------------------
// BOTONES
// ----------------------
byte buttonNow[3]  = {LOW, LOW, LOW};
byte buttonPrev[3] = {LOW, LOW, LOW};

// =====================================================
// FUNCIONES AUXILIARES
// =====================================================

void readButtons() {
  for (byte i = 0; i < 3; i++) {
    buttonNow[i] = digitalRead(buttonPins[i]);
  }
}

bool pressed(byte i) {
  return (buttonNow[i] == HIGH && buttonPrev[i] == LOW);
}

bool anyPressed() {
  return pressed(0) || pressed(1) || pressed(2);
}

void saveButtons() {
  for (byte i = 0; i < 3; i++) {
    buttonPrev[i] = buttonNow[i];
  }
}

void setGreenMask(byte mask) {
  for (byte i = 0; i < 3; i++) {
    digitalWrite(greenPins[i], (mask & (1 << i)) ? HIGH : LOW);
  }
}

void setRedMask(byte mask) {
  for (byte i = 0; i < 5; i++) {
    digitalWrite(redPins[i], (mask & (1 << i)) ? HIGH : LOW);
  }
}

void clearAllLeds() {
  setGreenMask(0);
  setRedMask(0);
}

byte scoreToRedMask(byte count) {
  byte mask = 0;
  for (byte i = 0; i < count && i < 5; i++) {
    mask |= (1 << i);
  }
  return mask;
}

void showDifficultyIndicator() {
  // Fácil = 1 verde, Medio = 2 verdes, Difícil = 3 verdes
  byte mask = (1 << difficulty) - 1;
  setGreenMask(mask);
  setRedMask(0);
}

void configureDifficulty(byte d) {
  difficulty = d;

  if (difficulty == 1) {
    reactionTimeMs = 1200UL;
    trapChance = 10;
  } else if (difficulty == 2) {
    reactionTimeMs = 600UL;
    trapChance = 28;
  } else {
    reactionTimeMs = 220UL;
    trapChance = 50;
  }
}

void restartToSelection() {
  score = 0;
  activeTopo = 255;
  trapActive = false;
  previousTopo = 255;

  finalRevealCount = 0;
  finalTargetReds = 0;
  finalMode = 1;
  finalBlinkState = false;

  clearAllLeds();
  state = SELECT_DIFFICULTY;
}

void startGame() {
  score = 0;
  activeTopo = 255;
  trapActive = false;
  previousTopo = 255;

  clearAllLeds();
  gameStartMs = millis();
  roundStartMs = gameStartMs;

  state = PLAYING;
}

byte chooseNextTopo(byte previous) {
  byte t = random(0, 3);
  while (t == previous) {
    t = random(0, 3);
  }
  return t;
}

bool chooseTrap() {
  return (random(0, 100) < trapChance);
}

void showCurrentTopo() {
  setGreenMask(1 << activeTopo);
}

void randomizeRedLocally() {
  for (byte i = 0; i < 5; i++) {
    digitalWrite(redPins[i], random(0, 2));
  }
}

void startNewRound() {
  previousTopo = activeTopo;
  activeTopo = chooseNextTopo(previousTopo);
  trapActive = chooseTrap();
  roundStartMs = millis();
  finalEffectMs = roundStartMs;
  finalBlinkState = false;

  showCurrentTopo();

  if (trapActive) {
    randomizeRedLocally();
  } else {
    setRedMask(0);
  }
}

void processHit(byte idx) {
  // Botón incorrecto o botón presionado que no corresponde
  if (idx != activeTopo) {
    score--;
    if (score < 0) score = 0;
    return;
  }

  // Botón correcto
  if (trapActive) {
    score = score / 2;
    if (score < 0) score = 0;
  } else {
    score++;
  }

  startNewRound();
}

void updateTrapVisual() {
  if (!trapActive) return;

  unsigned long now = millis();
  if (now - finalEffectMs >= TRAP_BLINK_MS) {
    finalEffectMs = now;
    randomizeRedLocally();
    showCurrentTopo();
  }
}

void startFinalSequence() {
  clearAllLeds();

  finalRevealCount = 0;
  finalTargetReds = score / 10;
  if (finalTargetReds > 5) finalTargetReds = 5;

  finalStepMs = millis();
  finalEffectMs = finalStepMs;
  finalBlinkState = false;
  finalHoldStartMs = 0;

  state = FINAL_REVEAL;
}

void updateFinalReveal() {
  unsigned long now = millis();

  if (finalRevealCount < finalTargetReds) {
    if (now - finalStepMs >= REVEAL_STEP_MS) {
      digitalWrite(redPins[finalRevealCount], HIGH);
      finalRevealCount++;
      finalStepMs = now;
    }
  } else {
    // Ya terminó de mostrar el resultado normal, ahora decide el modo loco
    if (score > 80) {
      finalMode = 3;
    } else if (score > 50) {
      finalMode = 2;
    } else {
      finalMode = 1;
    }

    finalHoldStartMs = now;
    finalEffectMs = now;
    finalBlinkState = false;

    if (finalMode == 1) {
      setRedMask(scoreToRedMask(finalTargetReds));
    } else if (finalMode == 2) {
      setRedMask(0b11111);
    } else {
      randomizeRedLocally();
    }

    state = FINAL_HOLD;
  }
}

void updateFinalHold() {
  unsigned long now = millis();

  if (finalMode == 1) {
    // Resultado normal fijo
    setRedMask(scoreToRedMask(finalTargetReds));
    return;
  }

  if (finalMode == 2) {
    // Parpadeo loco
    unsigned long interval = 140UL;
    if (score > 60) interval = 90UL;

    if (now - finalEffectMs >= interval) {
      finalEffectMs = now;
      finalBlinkState = !finalBlinkState;
      setRedMask(finalBlinkState ? 0b11111 : 0);
    }
    return;
  }

  if (finalMode == 3) {
    // Caos total
    unsigned long interval = 55UL;
    if (now - finalEffectMs >= interval) {
      finalEffectMs = now;
      randomizeRedLocally();
    }
    return;
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  for (byte i = 0; i < 3; i++) {
    pinMode(greenPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT); // si tus botones usan pull-up, cambia a INPUT_PULLUP
  }

  for (byte i = 0; i < 5; i++) {
    pinMode(redPins[i], OUTPUT);
  }

  clearAllLeds();
  state = SELECT_DIFFICULTY;
}

// =====================================================
// LOOP
// =====================================================
void loop() {
  readButtons();
  unsigned long now = millis();

  // 1) Elegir dificultad
  if (state == SELECT_DIFFICULTY) {
    clearAllLeds();

    if (pressed(0)) {
      configureDifficulty(1);
      showDifficultyIndicator();
      state = WAIT_START;
    } else if (pressed(1)) {
      configureDifficulty(2);
      showDifficultyIndicator();
      state = WAIT_START;
    } else if (pressed(2)) {
      configureDifficulty(3);
      showDifficultyIndicator();
      state = WAIT_START;
    }
  }

  // 2) Esperar inicio
  else if (state == WAIT_START) {
    showDifficultyIndicator();

    if (anyPressed()) {
      startGame();
    }
  }

  // 3) Jugando
  else if (state == PLAYING) {
    if (now - gameStartMs >= GAME_TIME_MS) {
      startFinalSequence();
    } else {
      // Cambia el topo si pasó el tiempo de reacción
      if (now - roundStartMs >= reactionTimeMs) {
        startNewRound();
      }

      // Mantener la visual del topo actual
      showCurrentTopo();
      updateTrapVisual();

      // Leer botones
      if (pressed(0)) {
        processHit(0);
      } else if (pressed(1)) {
        processHit(1);
      } else if (pressed(2)) {
        processHit(2);
      }
    }
  }

  // 4) Revelado final uno por uno
  else if (state == FINAL_REVEAL) {
    updateFinalReveal();
  }

  // 5) Locura final / marcador lleno
  else if (state == FINAL_HOLD) {
    updateFinalHold();

    // Bloqueo de botones hasta que pase un pequeño rato
    if (now - finalHoldStartMs >= FINAL_RESTART_LOCK_MS) {
      if (anyPressed()) {
        restartToSelection();
      }
    }
  }

  saveButtons();
}