# XiaoZhi AI — форк для платы Guition JC1060P470C

Русскоязычная сборка голосового ассистента [XiaoZhi](https://github.com/78/xiaozhi-esp32),
сокращённая до единственной целевой платы **Guition JC1060P470C** (ESP32-P4 + ESP32-C6).
Все остальные конфигурации плат удалены из проекта, чтобы уменьшить объём данных и
ускорить сборку.

## Железо (JC1060P470C)

| Компонент | Описание |
|-----------|----------|
| Основной MCU | ESP32-P4 (Rev v1.0/v1.3), Wi-Fi через ESP32-C6 (ESP-Hosted, SDIO) |
| Дисплей | 7" IPS 1024×600, панель JD9165 (MIPI-DSI, 2-lane) |
| Тачскрин | GT911 (I²C) |
| Аудио | ES8311 + усилитель (PA = GPIO20) |
| Камера | MIPI-CSI (EspVideo) |
| PSRAM | 32 МБ |

Подробности по пинам и ревизиям: [main/boards/guition-jc1060p470/README.md](main/boards/guition-jc1060p470/README.md)

## Сборка

Требуется **ESP-IDF v6.0.2** (рекомендуется Docker-образ `esp/idf:v6.0.2`):

```bash
idf.py set-target esp32p4
idf.py menuconfig   # Плата: Guition JC1060P470C (выбрана по умолчанию)
idf.py build
idf.py -p /dev/ttyACM0 flash
```

Тип платы зафиксирован в `main/Kconfig.projbuild` — других вариантов нет.

## CI

GitHub Actions собирает прошивку автоматически при пуше в ветку
`feature/jc1060p470c` (файлы `.github/workflows/build.yml` и
`.github/workflows/build-jc1060p470c.yml`). Готовые `.bin` доступны как
артефакты сборки (`esp32-firmware`).

## Что изменено по сравнению с апстримом

- Удалены все платы, кроме `guition-jc1060p470` (каталоги `main/boards/*`, кроме `common/`).
- `main/Kconfig.projbuild`: оставлен только выбор платы JC1060P470C, тексты переведены на русский.
- `main/CMakeLists.txt`: убраны ветви выбора чужих плат и блоки ESP-VoCat / ESP-HI.
- Язык интерфейса по умолчанию — русский (`LANGUAGE_RU_RU`).
- Документация и комментарии переведены на русский язык.

## Лицензия

Лицензия проекта — см. файл LICENSE (Apache-2.0, как в апстриме).
