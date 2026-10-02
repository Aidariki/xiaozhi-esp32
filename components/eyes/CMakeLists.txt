# Компонент «Интерактивные глаза» для платы Guition JC1060P470C.
# LVGL-слой с анимированными глазами; всегда доступен как компонент,
# но eyes.cc компилируется в пустую единицу трансляции, если выбран
# штатный облик глаз (см. guards по CONFIG_EYES_TYPE_* в eyes.cc).
idf_component_register(SRCS "eyes.cc"
                       INCLUDE_DIRS "."
                       REQUIRES lvgl)
