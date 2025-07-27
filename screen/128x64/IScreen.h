#pragma once

// Forward declaration
class ScreenManager;

class IScreen {
public:
  virtual ~IScreen() = default;

  // Отображение содержимого на дисплее
  virtual void draw(ScreenManager* manager) = 0;

  // Заголовок текущего экрана (строка 3 на макете)
  virtual const char* getTitle() = 0;

  // Есть ли вложенная логика — экран настроек и т.п.
  virtual bool isInteractive() { return false; }

  // Является ли экран полноэкранным
  virtual bool isFullScreen() { return false; }

  // Обработка клика ручки
  virtual void onRotaryClick() {}

  // Обработка двойного клика ручки
  virtual void onRotaryDoubleClick() {}

  // Обработка прокрутки (например, для выбора пункта)
  virtual void onRotate(int direction) {}
};
