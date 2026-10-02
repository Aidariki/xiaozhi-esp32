// Реализация интерактивных глаз (варианты из сообщества xiaozhi-esp32 /
// M5Stack CoreS3 / LVGL examples / xiaozhi-esp32-lcd-example).
// Если выбран штатный облик глаз (CONFIG_USE_EYES не определён),
// компонент собирается в «пустой» объект (см. CMakeLists.txt).
#ifdef CONFIG_USE_EYES
#include "eyes.h"

#include <esp_log.h>
#include <vector>

#define TAG "Eyes"

namespace eyes {

struct EyeWidgets {
    lv_obj_t* box = nullptr;      // контейнер глаза (маска, скругление)
    lv_obj_t* pupil = nullptr;    // зрачок
    int pupil_base_w = 0;         // базовые размеры зрачка для настроений
    int pupil_base_h = 0;
};

struct EyesState {
    EyesType type = EyesType::kStandard;
    Mood mood = Mood::kNeutral;
    int eye_w = 0, eye_h = 0;
    std::vector<EyeWidgets> eyes;
    lv_timer_t* blink_timer = nullptr;
    lv_timer_t* restore_timer = nullptr;
    bool animations_on = true;
};

static void apply_mood(EyesState* st);

static inline uint32_t mood_color(Mood m) {
    switch (m) {
        case Mood::kHappy:  return 0xFFD54F;  // янтарный
        case Mood::kSad:    return 0x64B5F6;  // голубоватый
        case Mood::kAngry:  return 0xEF5350;  // красный
        case Mood::kSleepy: return 0xBDBDBD;  // серый
        default:            return 0x2196F3;  // синий (нейтральный)
    }
}

// Вспомогательная настройка «плоского» объекта-фигуры
static lv_obj_t* make_shape(lv_obj_t* parent, int w, int h, uint32_t color, int radius) {
    lv_obj_t* o = lv_obj_create(parent);
    lv_obj_set_size(o, w > 0 ? w : LV_SIZE_CONTENT, h > 0 ? h : LV_SIZE_CONTENT);
    lv_obj_set_style_bg_color(o, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(o, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(o, 0, 0);
    lv_obj_set_style_radius(o, radius, 0);
    lv_obj_set_style_pad_all(o, 0, 0);
    lv_obj_clear_flag(o, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    return o;
}

// --- Моргание: раз в заданный интервал «закрываем» глаза на 150 мс -------
static void blink_restore_cb(lv_timer_t* rt) {
    EyesState* st = static_cast<EyesState*>(lv_timer_get_user_data(rt));
    for (auto& e : st->eyes) {
        if (e.pupil) lv_obj_remove_state(e.pupil, LV_STATE_DISABLED);
    }
    lv_timer_delete(rt);
    st->restore_timer = nullptr;
}

static void blink_cb(lv_timer_t* t) {
    EyesState* st = static_cast<EyesState*>(lv_timer_get_user_data(t));
    if (!st->animations_on) return;
    for (auto& e : st->eyes) {
        if (e.pupil) lv_obj_add_state(e.pupil, LV_STATE_DISABLED);
    }
    if (st->restore_timer == nullptr) {
        st->restore_timer = lv_timer_create(blink_restore_cb, 150, st);
        lv_timer_set_repeat_count(st->restore_timer, 1);
    }
}

// --- Построение одного глаза ----------------------------------------------
static EyeWidgets make_eye(lv_obj_t* box, EyesState* st) {
    EyeWidgets ew;
    const int w = lv_obj_get_width(box);
    const int h = lv_obj_get_height(box);
    const uint32_t mc = mood_color(st->mood);

    ew.box = box;
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_pad_all(box, 0, 0);
    lv_obj_set_style_border_width(box, 0, 0);
    lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);

    switch (st->type) {
        case EyesType::kClassic: {
            // белый круг с тёмной окантовкой + цветной зрачок
            lv_obj_set_style_radius(box, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_color(box, lv_color_white(), 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
            lv_obj_set_style_border_width(box, 3, 0);
            lv_obj_set_style_border_color(box, lv_color_hex(0x333333), 0);
            ew.pupil_base_w = w * 45 / 100;
            ew.pupil_base_h = h * 45 / 100;
            ew.pupil = make_shape(box, ew.pupil_base_w, ew.pupil_base_h, mc, LV_RADIUS_CIRCLE);
            break;
        }
        case EyesType::kRobot: {
            // тёмный визор + светящаяся полоса-зрачок
            lv_obj_set_style_radius(box, 8, 0);
            lv_obj_set_style_bg_color(box, lv_color_hex(0x111111), 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
            ew.pupil_base_w = w * 60 / 100;
            ew.pupil_base_h = h * 30 / 100;
            ew.pupil = make_shape(box, ew.pupil_base_w, ew.pupil_base_h, mc, 4);
            break;
        }
        case EyesType::kPixel: {
            // квадрат с неоновой рамкой + квадратный зрачок
            lv_obj_set_style_radius(box, 0, 0);
            lv_obj_set_style_bg_color(box, lv_color_black(), 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
            lv_obj_set_style_border_width(box, 4, 0);
            lv_obj_set_style_border_color(box, lv_color_hex(0x00E676), 0);
            ew.pupil_base_w = w * 50 / 100;
            ew.pupil_base_h = h * 50 / 100;
            ew.pupil = make_shape(box, ew.pupil_base_w, ew.pupil_base_h, 0x00E676, 0);
            break;
        }
        case EyesType::kAnime: {
            // крупная радужка + белый блик + ресницы
            lv_obj_set_style_radius(box, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_color(box, lv_color_white(), 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
            lv_obj_set_style_border_width(box, 4, 0);
            lv_obj_set_style_border_color(box, lv_color_hex(0x4A148C), 0);
            ew.pupil_base_w = w * 55 / 100;
            ew.pupil_base_h = h * 55 / 100;
            ew.pupil = make_shape(box, ew.pupil_base_w, ew.pupil_base_h, mc, LV_RADIUS_CIRCLE);
            lv_obj_align(ew.pupil, LV_ALIGN_CENTER, 0, h * 5 / 100);
            lv_obj_t* hl = make_shape(ew.pupil, w * 18 / 100, h * 18 / 100, 0xFFFFFF, LV_RADIUS_CIRCLE);
            lv_obj_align(hl, LV_ALIGN_TOP_LEFT, ew.pupil_base_w * 15 / 100, ew.pupil_base_h * 15 / 100);
            for (int i = -1; i <= 1; ++i) {
                lv_obj_t* lash = make_shape(box, 6, h * 25 / 100, 0x4A148C, 3);
                lv_obj_align(lash, LV_ALIGN_TOP_MID, i * (w * 25 / 100), -(h * 22 / 100));
            }
            break;
        }
        case EyesType::kKawaii:
        default: {
            // градиентный круг цвета настроения + маленький чёрный зрачок
            lv_obj_set_style_radius(box, LV_RADIUS_CIRCLE, 0);
            lv_obj_set_style_bg_color(box, lv_color_hex(mc), 0);
            lv_obj_set_style_bg_grad_color(box, lv_color_white(), 0);
            lv_obj_set_style_bg_grad_dir(box, LV_GRAD_DIR_VER, 0);
            lv_obj_set_style_bg_opa(box, LV_OPA_COVER, 0);
            ew.pupil_base_w = w * 35 / 100;
            ew.pupil_base_h = h * 35 / 100;
            ew.pupil = make_shape(box, ew.pupil_base_w, ew.pupil_base_h, 0x000000, LV_RADIUS_CIRCLE);
            break;
        }
    }

    if (ew.pupil) {
        lv_obj_center(ew.pupil);
        // «закрытый глаз» при моргании: тонкая линия вместо зрачка
        lv_obj_set_style_bg_color(ew.pupil, lv_color_hex(0x333333), LV_STATE_DISABLED);
        lv_obj_set_style_bg_grad_color(ew.pupil, lv_color_hex(0x333333), LV_STATE_DISABLED);
    }
    return ew;
}

// --- API -------------------------------------------------------------------
lv_obj_t* create_eyes_screen(lv_obj_t* parent, EyesType type) {
    auto* st = new EyesState();
    st->type = type;

    int pw = lv_obj_get_width(parent);
    int ph = lv_obj_get_height(parent);
    if (pw <= 0) pw = 1024;   // экран JC1060P470C
    if (ph <= 0) ph = 600;

    const int eye_w = pw / 6;
    const int eye_h = ph / 4;
    const int gap = pw / 8;
    st->eye_w = eye_w;
    st->eye_h = eye_h;

    for (int i = 0; i < 2; ++i) {
        lv_obj_t* box = lv_obj_create(parent);
        lv_obj_set_size(box, eye_w, eye_h);
        lv_obj_align(box, LV_ALIGN_CENTER, (i == 0 ? -(eye_w / 2 + gap / 2) : (eye_w / 2 + gap / 2)), 0);
        st->eyes.push_back(make_eye(box, st));
    }

    st->blink_timer = lv_timer_create(blink_cb, CONFIG_EYES_BLINK_INTERVAL_MS, st);

    // Маркер слоя глаз на родительском экране (для корректного удаления).
    lv_obj_add_flag(parent, LV_OBJ_FLAG_USER_1);
    lv_obj_set_user_data(parent, st);
    ESP_LOGI(TAG, "Интерактивные глаза созданы (тип=%d)", static_cast<int>(type));
    return parent;
}

void set_mood(lv_obj_t* screen, Mood mood) {
    auto* st = static_cast<EyesState*>(lv_obj_get_user_data(screen));
    if (!st) return;
    st->mood = mood;
    apply_mood(st);
}

void set_animations_enabled(lv_obj_t* screen, bool enabled) {
    auto* st = static_cast<EyesState*>(lv_obj_get_user_data(screen));
    if (!st) return;
    st->animations_on = enabled;
    if (st->blink_timer) {
        if (enabled) lv_timer_resume(st->blink_timer); else lv_timer_pause(st->blink_timer);
    }
}

void destroy(lv_obj_t* screen) {
    auto* st = static_cast<EyesState*>(lv_obj_get_user_data(screen));
    if (!st || !lv_obj_has_flag(screen, LV_OBJ_FLAG_USER_1)) return;  // не слой глаз
    if (st->blink_timer) lv_timer_delete(st->blink_timer);
    if (st->restore_timer) lv_timer_delete(st->restore_timer);
    // Удаляем только контейнеры глаз (с их детьми), штатный UI не трогаем.
    for (auto& e : st->eyes) {
        if (e.box) lv_obj_delete(e.box);
    }
    delete st;
    lv_obj_set_user_data(screen, nullptr);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_USER_1);
}

static void apply_mood(EyesState* st) {
    uint32_t c = mood_color(st->mood);
    int scale_pct = 100;
    int dy_pct = 0;
    switch (st->mood) {
        case Mood::kHappy:  scale_pct = 115; dy_pct = -5;  break;
        case Mood::kSad:    scale_pct = 85;  dy_pct = 10;  break;
        case Mood::kAngry:  scale_pct = 75;  dy_pct = 0;   break;
        case Mood::kSleepy: scale_pct = 45;  dy_pct = 15;  break;
        default: break;
    }
    for (auto& e : st->eyes) {
        if (e.pupil == nullptr) continue;
        lv_obj_set_size(e.pupil, e.pupil_base_w * scale_pct / 100, e.pupil_base_h * scale_pct / 100);
        lv_obj_align(e.pupil, LV_ALIGN_CENTER, 0, st->eye_h * dy_pct / 100);
        lv_obj_set_style_bg_color(e.pupil, lv_color_hex(c), 0);
        if (st->type == EyesType::kKawaii) {
            lv_obj_set_style_bg_color(e.box, lv_color_hex(c), 0);
        }
    }
}

}  // namespace eyes

#endif  // CONFIG_USE_EYES

