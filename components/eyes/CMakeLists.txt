# Компонент «Интерактивные глаза» для платы Guition JC1060P470C.
# LVGL-слой с анимированными глазами; собирается только если выбран
# нестандартный облик (CONFIG_USE_EYES), иначе eyes.cc даёт пустой объект.
idf_component_register(SRCS "eyes.cc"
                       INCLUDE_DIRS "."
                       REQUIRES lvgl)
