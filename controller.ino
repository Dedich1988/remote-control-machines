#include <IRremote.hpp> // Подключаем библиотеку IRremote

// Настройка пинов для ИК-датчика и моторов
#define IR_RECEIVE_PIN 2 // Пин, к которому подключен ИК-датчик
#define IN1 3            // Управление мотором 1 (направление)
#define IN2 4
#define IN3 5            // Управление мотором 2 (направление)
#define IN4 6

// Коды кнопок пульта
#define CMD_FORWARD 0xEA15FF00  // Вперед
#define CMD_BACKWARD 0xF807FF00 // Назад
#define CMD_STOP 0xBC43FF00     // Стоп
#define CMD_LEFT 0xBB44FF00     // Влево
#define CMD_RIGHT 0xBF40FF00    // Вправо

// Переменная для хранения текущего направления
enum Direction {
  STOP,       // Остановлен
  FORWARD,    // Вперед
  BACKWARD,   // Назад
  LEFT,       // Влево
  RIGHT       // Вправо
};
Direction currentDirection = STOP; // По умолчанию робот стоит

void setup() {
  // Настраиваем пины для моторов как выходы
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Настраиваем ИК-приемник
  IrReceiver.begin(IR_RECEIVE_PIN);
  Serial.begin(9600); // Для вывода данных на монитор
  Serial.println("Система готова к управлению!");
}

void loop() {
  // Проверяем, получен ли сигнал от ИК-пульта
  if (IrReceiver.decode()) {
    unsigned long command = IrReceiver.decodedIRData.decodedRawData; // Читаем код кнопки
    Serial.print("Получен код: ");
    Serial.println(command, HEX); // Выводим код в HEX для отладки

    // Определяем новое направление на основе команды
    switch (command) {
      case CMD_FORWARD:
        Serial.println("Команда: Вперед");
        currentDirection = FORWARD;
        break;

      case CMD_BACKWARD:
        Serial.println("Команда: Назад");
        currentDirection = BACKWARD;
        break;

      case CMD_LEFT:
        Serial.println("Команда: Влево");
        currentDirection = LEFT;
        break;

      case CMD_RIGHT:
        Serial.println("Команда: Вправо");
        currentDirection = RIGHT;
        break;

      case CMD_STOP:
        Serial.println("Команда: Стоп");
        currentDirection = STOP;
        break;

      default:
        Serial.println("Неизвестная команда");
        break;
    }

    // Готовимся к приему следующего сигнала
    IrReceiver.resume();
  }

  // Выполняем действие в зависимости от текущего направления
  switch (currentDirection) {
    case FORWARD:
      moveForward();
      break;

    case BACKWARD:
      moveBackward();
      break;

    case LEFT:
      turnLeft();
      break;

    case RIGHT:
      turnRight();
      break;

    case STOP:
    default:
      stopMotors();
      break;
  }
}

// Функция движения вперед
void moveForward() {
  digitalWrite(IN1, HIGH); // Мотор 1 вращается вперед
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); // Мотор 2 вращается вперед
  digitalWrite(IN4, LOW);
}

// Функция движения назад
void moveBackward() {
  digitalWrite(IN1, LOW); // Мотор 1 вращается назад
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); // Мотор 2 вращается назад
  digitalWrite(IN4, HIGH);
}

// Функция поворота влево
void turnLeft() {
  digitalWrite(IN1, LOW); // Мотор 1 вращается назад
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); // Мотор 2 вращается вперед
  digitalWrite(IN4, LOW);
}

// Функция поворота вправо
void turnRight() {
  digitalWrite(IN1, HIGH); // Мотор 1 вращается вперед
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); // Мотор 2 вращается назад
  digitalWrite(IN4, HIGH);
}

// Функция остановки моторов
void stopMotors() {
  digitalWrite(IN1, LOW); // Мотор 1 остановлен
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); // Мотор 2 остановлен
  digitalWrite(IN4, LOW);
}
