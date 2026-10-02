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
- Добавлены «Интерактивные глаза» (компонент `components/eyes`): 5 анимированных
  обликов (классические, робот-визоры, пиксельные, аниме, каваи) с морганием и
  реакцией на настроение ассистента. Выбор — в `menuconfig` → «Интерактивные глаза»,
  переключение коротким нажатием BOOT или голосом через MCP-навыки
  `self.eyes.set_style` / `self.eyes.get_style`. По умолчанию выбран стандартный
  облик — влияние на размер прошивки отсутствует.

## Как добавить свой навык агенту (MCP-инструмент)

Скиллами (навыками) агента в XiaoZhi управляет встроенный **MCP-сервер**
(`main/mcp_server.cc/.h`, протокол — JSON-RPC 2.0 поверх WebSocket/MQTT; подробно
в `docs/mcp-usage.md` и `docs/mcp-protocol.md`). Сервер сам отдаёт облаку список
инструментов (`tools/list`), и большая модель вызывает их (`tools/call`) — то есть
новый навык не нужно описывать на сайте xz: он «самообнаруживается» с прошивкой.

Регистрация навыка делается в функции платы `InitializeTools()`:

```cpp
auto& mcp_server = McpServer::GetInstance();
mcp_server.AddTool("self.light.set_rgb",        // имя: «модуль.действие»
    "Установить цвет RGB подсветки",            // описание для LLM (можно на русском)
    PropertyList({                               // параметры: int/bool/string (+диапазон)
        Property("r", kPropertyTypeInteger, 0, 255),
        Property("g", kPropertyTypeInteger, 0, 255),
        Property("b", kPropertyTypeInteger, 0, 255)
    }),
    [](const PropertyList& p) -> ReturnValue {   // тело навыка
        SetLedColor(p["r"].value<int>(), p["g"].value<int>(), p["b"].value<int>());
        return true;                             // bool/int/string/JSON
    });
```

Дополнительно: `AddUserOnlyTool()` — навык доступен только владельцу устройства
(через отладочное подключение); исключения из callback превращаются в текст ошибки
для модели.

## Навыки, доступные агенту прямо сейчас

Встроенные (всегда, `AddCommonTools`):
- `self.get_device_status` — статус устройства (динамик, экран, сеть, батарея)
- `self.audio_speaker.set_volume` — громкость динамика (0–100)
- `self.screen.set_brightness` — яркость экрана (есть подсветка — а JC1060P470C есть)
- `self.screen.set_theme` — тема экрана light/dark (LVGL-дисплеи)
- `self.camera.take_photo` + ответ на вопрос о фото (если плата возвращает камеру)

Только для владельца (`AddUserOnlyTools`): `self.get_system_info`, `self.reboot`,
`self.upgrade_firmware` (OTA по URL), `self.screen.get_info`, `self.screen.snapshot`
(скриншот на URL), `self.screen.preview_image`, `self.assets.set_download_url`.

Навыки этой сборки (JC1060P470C):
- `self.set_press_to_talk` — режим push-to-talk / click-to-talk (общий модуль плат)
- `self.eyes.set_style` / `self.eyes.get_style` — новые интерактивные глаза
  (активны, если в menuconfig выбран нестандартный облик)

## Лицензия

Лицензия проекта — см. файл LICENSE (Apache-2.0, как в апстриме).
