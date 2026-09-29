# Использование MCP для управления IoT-устройствами

> В этом документе описано, как реализовать управление IoT для устройств на ESP32 с помощью протокола MCP. Подробное описание протокола передачи см. в [`mcp-protocol.md`](./mcp-protocol.md).

## Введение

MCP (Model Context Protocol) — рекомендуемый протокол для управления IoT в этом проекте. Он использует JSON-RPC 2.0, позволяя бэкенду обнаруживать «инструменты» (tools), зарегистрированные устройством, и вызывать их, давая гибкий способ опубликовать функциональность устройства.

## Типичный поток

1. Устройство загружается и подключается к бэкенду по WebSocket или MQTT.
2. Бэкенд отправляет вызов `initialize` для начала сессии MCP.
3. Бэкенд выполняет `tools/list`, чтобы обнаружить доступные инструменты и их схемы входных параметров.
4. Бэкенд вызывает отдельные инструменты через `tools/call` для управления устройством.

Точный формат сообщений см. в [`mcp-protocol.md`](./mcp-protocol.md).

## Регистрация инструментов на устройстве

Инструменты регистрируются через синглтон `McpServer`. Есть два API регистрации:

- `McpServer::AddTool` — обычный инструмент: виден в ответе `tools/list` по умолчанию и может вызываться ИИ-моделью.
- `McpServer::AddUserOnlyTool` — скрытый инструмент: возвращается только тогда, когда бэкенд запрашивает список инструментов с `withUserTools=true`. Используйте его для привилегированных или инициируемых пользователем действий (перезагрузка, обновление прошивки, снимки экрана и т. п.), которые модель не должна вызывать самостоятельно.

Оба API имеют одинаковую сигнатуру:

```cpp
void AddTool(
    const std::string& name,           // уникальное имя инструмента, напр. self.dog.forward
    const std::string& description,    // краткое описание для модели
    const PropertyList& properties,    // входные параметры (могут быть пустыми); поддерживаемые типы: bool, int, string
    std::function<ReturnValue(const PropertyList&)> callback // реализация
);

void AddUserOnlyTool(
    const std::string& name,
    const std::string& description,
    const PropertyList& properties,
    std::function<ReturnValue(const PropertyList&)> callback
);
```

- `name` — уникальный идентификатор. Хорошо подходит стиль именования `модуль.действие`.
- `description` — описание на естественном языке; ИИ использует его, чтобы решить, когда вызывать инструмент.
- `properties` — входные параметры. Поддерживаются логический, целочисленный и строковый типы, а также необязательные минимум/максимум и значения по умолчанию.
- `callback` — реализация. Возвращаемое значение может иметь тип `bool`, `int` или `std::string`.

## Пример (ESP-HI)

```cpp
void InitializeTools() {
    auto& mcp_server = McpServer::GetInstance();

    // Пример 1: без аргументов — движение робота вперёд
    mcp_server.AddTool("self.dog.forward",
        "Move the robot forward",
        PropertyList(),
        [this](const PropertyList&) -> ReturnValue {
            servo_dog_ctrl_send(DOG_STATE_FORWARD, NULL);
            return true;
        });

    // Пример 2: с аргументами — установка цвета RGB-светодиода
    mcp_server.AddTool("self.light.set_rgb",
        "Set the RGB color of the light",
        PropertyList({
            Property("r", kPropertyTypeInteger, 0, 255),
            Property("g", kPropertyTypeInteger, 0, 255),
            Property("b", kPropertyTypeInteger, 0, 255)
        }),
        [this](const PropertyList& properties) -> ReturnValue {
            int r = properties["r"].value<int>();
            int g = properties["g"].value<int>();
            int b = properties["b"].value<int>();
            led_on_ = true;
            SetLedColor(r, g, b);
            return true;
        });
}
```

## Пример — регистрация инструмента только для пользователя

```cpp
mcp_server.AddUserOnlyTool("self.display.clear_cache",
    "Clear locally cached images. User-only action.",
    PropertyList(),
    [](const PropertyList&) -> ReturnValue {
        ClearLocalCache();
        return true;
    });
```

Инструмент, зарегистрированный таким образом, не появится в обычном ответе `tools/list`. Бэкенд должен установить `params.withUserTools = true`, чтобы увидеть его.

## Встроенные инструменты

`McpServer::AddCommonTools` и `McpServer::AddUserOnlyTools` автоматически регистрируют ряд инструментов:

### Инструменты по умолчанию (вызываемые ИИ) — из `AddCommonTools`

| Инструмент | Описание |
|------|-------------|
| `self.get_device_status` | Возвращает текущую громкость, состояние экрана, батареи, сети и т. д. |
| `self.audio_speaker.set_volume` | Устанавливает громкость динамика (`volume`: 0–100). |
| `self.screen.set_brightness` | Устанавливает яркость экрана, если доступна подсветка (`brightness`: 0–100). |
| `self.screen.set_theme` | Переключает тему UI (`theme`: `"light"` или `"dark"`), если включён LVGL. |
| `self.camera.take_photo` | Делает снимок встроенной камерой (если она есть на плате) и отвечает на заданный `question` об этом снимке. |

Специфичные для платы инструменты добавляются после этих в `InitializeTools()` каждой платы.

### Инструменты только для пользователя — из `AddUserOnlyTools`

Эти инструменты по умолчанию скрыты. Бэкенд должен передать `withUserTools=true` в `tools/list`, чтобы их увидеть. Они предназначены для сопутствующих приложений / конечных пользователей, а не для ИИ-модели.

| Инструмент | Описание |
|------|-------------|
| `self.get_system_info` | Возвращает JSON-объект с описанием системы. |
| `self.reboot` | Перезагружает устройство после небольшой задержки. |
| `self.upgrade_firmware` | Загружает прошивку по `url` и устанавливает её, затем перезагружается. |
| `self.screen.get_info` | Возвращает текущие ширину и высоту экрана и признак монохромности (только платы с LVGL). |
| `self.screen.snapshot` | Делает снимок экрана в JPEG и загружает его по `url` (платы с LVGL, при `CONFIG_LV_USE_SNAPSHOT=y`). |
| `self.screen.preview_image` | Загружает изображение по `url` и отображает его на экране. |
| `self.assets.set_download_url` | Устанавливает URL загрузки для раздела ресурсов (assets). |

## Примеры JSON-RPC

### 1. Получить список инструментов

```json
{
  "jsonrpc": "2.0",
  "method": "tools/list",
  "params": { "cursor": "", "withUserTools": false },
  "id": 1
}
```

### 2. Движение шасси вперёд

```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "self.chassis.go_forward",
    "arguments": {}
  },
  "id": 2
}
```

### 3. Переключение режима света

```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "self.chassis.switch_light_mode",
    "arguments": { "light_mode": 3 }
  },
  "id": 3
}
```

### 4. Перезагрузка устройства (только для пользователя)

```json
{
  "jsonrpc": "2.0",
  "method": "tools/call",
  "params": {
    "name": "self.reboot",
    "arguments": {}
  },
  "id": 4
}
```

## Примечания

- Названия инструментов, параметры и возвращаемые значения должны совпадать с тем, что устройство регистрирует через `AddTool` / `AddUserOnlyTool`.
- Для любого нового IoT-управления предпочитайте MCP.
- Протокол передачи и дополнительные темы см. в [`mcp-protocol.md`](./mcp-protocol.md).
