// Обёртка «Интерактивные глаза» поверх стандартного LVGL-экрана дисплея.
// Позволяет не менять базовые классы Display/LcdDisplay: слой глаз
// прикрепляется к активному экрану, а смена настроения перехватывается
// через виртуальный метод SetEmotion (см. EyesLcdDisplay).
#pragma once

#include "lcd_display.h"
#include "settings.h"

// Заголовок глаз подключается ВСЕГДА (объявления eyes:: не зависят от
// Kconfig), иначе eyes.cc и eyes_display.cc компилировались бы в
// несогласованные переводы единиц (один с реализацией, другой без) —
// отсюда «undefined reference to eyes::...» на этапе линковки.
// Реализации функций eyes:: компилируются только при выбранном
// нестандартном облике — см. #if в eyes.cc / eyes_display.cc.
#include "eyes.h"

class EyesLcdDisplay : public MipiLcdDisplay {
public:
    EyesLcdDisplay(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel, int width,
                   int height, int offset_x, int offset_y, bool mirror_x, bool mirror_y,
                   bool swap_xy)
        : MipiLcdDisplay(panel_io, panel, width, height, offset_x, offset_y, mirror_x, mirror_y,
                         swap_xy) {
        // Восстанавливаем ранее выбранный облик глаз из настроек.
        Settings settings("interface", false);
        eyes_type_ = settings.GetInt("eyes_type", CONFIG_EYES_TYPE_ID_DEFAULT);
    }

    // Создать слой глаз поверх штатного UI (вызывать после SetupUI()).
    void EnableEyes();
    // Удалить слой глаз (возврат к стандартному облику).
    void DisableEyes();
    // Переключить облик на следующий по кругу (0..5), сохранить в настройки.
    void CycleEyesType() { SetEyesType((eyes_type_ + 1) % 6); }

    // Явно задать облик глаз (0..5), сохранить в настройки. Возвращает новый тип.
    int SetEyesType(int t) {
        if (t < 0 || t > 5) return eyes_type_;
        eyes_type_ = t;
        Settings settings("interface", true);
        settings.SetInt("eyes_type", eyes_type_);
        DisableEyes();
        if (eyes_type_ != 0) EnableEyes();
        return eyes_type_;
    }

    // Текущий выбранный тип глаз (0 = стандартный).
    int CurrentEyesType() const { return eyes_type_; }

    // Имена обликов для уведомлений и MCP-описаний (индекс = тип 0..5).
    static const char* EyesTypeName(int t);

    virtual void SetEmotion(const char* emotion) override;

private:
    // Слой глаз (nullptr, если облик отключён на этапе компиляции)
    lv_obj_t* eyes_layer_ = nullptr;
    int eyes_type_ = 0;
};

