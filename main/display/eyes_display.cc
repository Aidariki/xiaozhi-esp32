// Реализация обёртки «Интерактивные глаза».
#include "eyes_display.h"

#ifdef CONFIG_USE_EYES
#include <esp_log.h>
#include <lvgl.h>
#include "settings.h"

#define TAG "EyesDisplay"

// Соответствие CONFIG_EYES_TYPE_* -> eyes::EyesType
static eyes::EyesType ConfigToEyesType(int t) {
    switch (t) {
        case 1: return eyes::EyesType::kClassic;
        case 2: return eyes::EyesType::kRobot;
        case 3: return eyes::EyesType::kPixel;
        case 4: return eyes::EyesType::kAnime;
        case 5: return eyes::EyesType::kKawaii;
        default: return eyes::EyesType::kStandard;
    }
}

void EyesLcdDisplay::EnableEyes() {
    if (eyes_type_ == 0) return;  // Стандартный облик — слой не нужен
    if (eyes_layer_ != nullptr) return;  // уже включено
    DisplayLockGuard guard(this);
    lv_obj_t* scr = lv_screen_active();
    if (scr == nullptr) {
        ESP_LOGW(TAG, "Нет активного экрана LVGL — глаза не созданы");
        return;
    }
    // Слой поверх штатного UI: прозрачный фон, не перехватывает касания.
    eyes_layer_ = scr;
    eyes::create_eyes_screen(scr, ConfigToEyesType(eyes_type_));
    ESP_LOGI(TAG, "Слой глаз включён (тип=%d)", eyes_type_);
}

void EyesLcdDisplay::DisableEyes() {
    if (eyes_layer_ == nullptr) return;
    DisplayLockGuard guard(this);
    eyes::destroy(eyes_layer_);
    eyes_layer_ = nullptr;
}

void EyesLcdDisplay::SetEmotion(const char* emotion) {
    MipiLcdDisplay::SetEmotion(emotion);
    if (eyes_layer_ == nullptr || emotion == nullptr) return;
    DisplayLockGuard guard(this);
    eyes::Mood mood = eyes::Mood::kNeutral;
    std::string e(emotion);
    if (e == "happy" || e == "laughing" || e == "funny") mood = eyes::Mood::kHappy;
    else if (e == "sad" || e == "crying") mood = eyes::Mood::kSad;
    else if (e == "angry") mood = eyes::Mood::kAngry;
    else if (e == "sleepy" || e == "relaxed") mood = eyes::Mood::kSleepy;
    eyes::set_mood(eyes_layer_, mood);
}
#else
// Стандартный облик: обёртка не добавляет ничего сверх базового класса.
void EyesLcdDisplay::EnableEyes() {}
void EyesLcdDisplay::DisableEyes() {}
void EyesLcdDisplay::SetEmotion(const char* emotion) {
    MipiLcdDisplay::SetEmotion(emotion);
}
#endif
