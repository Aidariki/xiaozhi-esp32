// Интерфейс компонента «Интерактивные глаза».
// Объявления доступны всегда; реализации компилируются только при
// выбранном в конфигураторе нестандартном облике глаз
// (CONFIG_EYES_TYPE_*, см. main/CMakeLists.txt).
#pragma once
#include <lvgl.h>

namespace eyes {

// Типы глаз (значения совпадают с CONFIG_EYES_TYPE_* из Kconfig).
enum class EyesType {
    kStandard = 0,  // Стандартный облик XiaoZhi (эмодзи/иконки)
    kClassic,       // Классические круглые глаза (m5stack-cores3 style)
    kRobot,         // Robot-style: широкие «visor»-зрачки (ESP32-CAM bot style)
    kPixel,         // Пиксельные квадратные глаза (retro/pixel-art style)
    kAnime,         // Аниме-глаза с ресницами и бликами (LVGL EYES example style)
    kKawaii,        // Каваи-глаза «arco» (xiaozhi-esp32-lcd-example style)
};

enum class Mood {
    kNeutral,
    kHappy,
    kSad,
    kAngry,
    kSleepy,
};

// Создать экран с глазами выбранного типа.
// Родительский объект — любой контейнер (например, active screen).
// Размер области задаётся шириной/высотой родителя; глаза центрируются.
lv_obj_t* create_eyes_screen(lv_obj_t* parent, EyesType type);

// Сменить настроение (открывает/закрывает зрачки, наклон «бровей»).
void set_mood(lv_obj_t* screen, Mood mood);

// Включить/выключить анимацию (мерцание, моргание, движение зрачков).
void set_animations_enabled(lv_obj_t* screen, bool enabled);

// Освободить ресурсы экрана (удаляет LVGL-объект и таймеры).
void destroy(lv_obj_t* screen);

}  // namespace eyes
