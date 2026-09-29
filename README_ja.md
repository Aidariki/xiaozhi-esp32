# Чат-бот на базе MCP

(【японская версия】日本語 | [中文](README_zh.md) | [Русский](README.md) | [English](https://github.com/78/xiaozhi-esp32/blob/main/README.md))

## Введение

👉 [Человек: даёт ИИ камеру, а ИИ: тут же выяснил, что хозяин не мыл голову три дня【bilibili】](https://www.bilibili.com/video/BV1bpjgzKEhd/)

👉 [Создаём свою ИИ-девушку своими руками: вводный урок для новичков【bilibili】](https://www.bilibili.com/video/BV1XnmFYLEJN/)

AI-чат-бот Сяо Чжи (XiaoZhi), будучи точкой голосового взаимодействия, использует возможности больших моделей, таких как Qwen / DeepSeek, и реализует управление множеством конечных устройств через протокол MCP.

<img src="docs/mcp-based-graph.jpg" alt="Управление всем через MCP" width="320">

## Последние обновления

- Основная ветвь переведена на ESP-IDF v6.0 и новее; рекомендуемая стабильная версия — v6.0.2. Прежние 157 релизных вариантов были проверены сборкой на ESP-IDF v6.0.1. Текущая матрица содержит 171 вариант, из которых 170 поддерживают IDF 6.0.x, а для варианта ESP32-S31 требуется IDF 6.1 или новее.
- Криптографические операции MQTT и BluFi перенесены на PSA Crypto; также выполнены работы по разделению компонентов в IDF 6 и совместимости сторонних зависимостей.
- Усовершенствованы параллельность аудиоконвейера, проверка пакетов MQTT/UDP и логика выбора релизной матрицы.
- ESP-IDF v5.5 сохранён только для старых плат, явно указанных в документации. При использовании ESP-SR 2.4.7 платы ESP32-P4 Rev1 и Rev3 поддерживаются на IDF 6. Подробную информацию о совместимости и статусе проверки плат см. в [Руководстве по миграции на ESP-IDF 6.0](docs/esp-idf-6-migration.md).

### Реализованные функции

- Поддержка Wi-Fi, проводного Ethernet, USB RNDIS, а также 4G-сетей ML307/EC801E или NT26 Cat.1; на некоторых платах возможно переключение между Wi-Fi и 4G
- Офлайн-голосовая активация средствами [ESP-SR](https://github.com/espressif/esp-sr) и пользовательские слова активации
- Два способа связи: [WebSocket](docs/websocket.md) и [MQTT + UDP](docs/mqtt-udp.md)
- Аудиопоток Opus поддерживает как традиционную схему «потоковые ASR + LLM + TTS», так и end-to-end realtime-голосовые модели; на оборудовании с AEC доступен realtime-диалог в полном дуплексе
- Распознавание голоса: определяет, кто сейчас говорит [3D Speaker](https://github.com/modelscope/3D-Speaker)
- Дисплеи OLED / LCD с отображением эмодзи и богатой эмоциональной выразительности; на некоторых платах поддерживается визуальный ввод с камеры
- Отображение заряда аккумулятора и управление электропитанием
- 38 языков интерфейса; голосовые подсказки используют локализованные ресурсы при их наличии, с откатом на английский
- Платформы чипов ESP32, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-S3, ESP32-P4
- Настройка Wi-Fi через точку доступа или BluFi
- Управление устройством через MCP на стороне устройства (громкость, яркость, выполнение действий и т. п.)
- Расширение возможностей большой модели через облачный MCP (управление умным домом, операции на рабочем столе ПК, поиск знаний, отправка и получение почты и т. п.)
- Настраиваемые слова активации, шрифты, эмодзи и фоны чата с онлайн-редактированием через веб-страницу ([Генератор пользовательских ресурсов](https://github.com/78/xiaozhi-assets-generator))

## Оборудование

### Сборка своими руками на макетной плате

См. руководство в документе Feishu:

👉 [«Энциклопедия AI-чат-бота Сяо Чжи»](https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb?from=from_copylink)

Демонстрация на макетной плате:

![Демонстрация на макетной плате](docs/v1/wiring2.jpg)

### Поддержка 138 каталогов плат и 171 релизного варианта (показана только часть)

- <a href="https://oshwhub.com/li-chuang-kai-fa-ban/li-chuang-shi-zhan-pai-esp32-s3-kai-fa-ban" target="_blank" title="Плата разработки LiChuang ESP32-S3">Плата разработки LiChuang ESP32-S3</a>
- <a href="https://github.com/espressif/esp-box" target="_blank" title="Espressif (乐鑫) ESP32-S3-BOX-3">Espressif ESP32-S3-BOX-3</a>
- <a href="https://docs.m5stack.com/zh_CN/core/CoreS3" target="_blank" title="M5Stack CoreS3">M5Stack CoreS3</a>
- <a href="https://docs.m5stack.com/en/atom/Atomic%20Echo%20Base" target="_blank" title="AtomS3R + Echo Base">M5Stack AtomS3R + Echo Base</a>
- <a href="https://gf.bilibili.com/item/detail/1108782064" target="_blank" title="Волшебная кнопка 2.4">Волшебная кнопка 2.4</a>
- <a href="https://www.waveshare.net/shop/ESP32-S3-Touch-AMOLED-1.8.htm" target="_blank" title="Waveshare (微雪电子) ESP32-S3-Touch-AMOLED-1.8">Waveshare ESP32-S3-Touch-AMOLED-1.8</a>
- <a href="https://github.com/Xinyuan-LilyGO/T-Circle-S3" target="_blank" title="LILYGO T-Circle-S3">LILYGO T-Circle-S3</a>
- <a href="https://oshwhub.com/tenclass01/xmini_c3" target="_blank" title="Mini C3 от сяо Гэ (брата-креветки)">Mini C3 от сяо Гэ</a>
- <a href="https://oshwhub.com/movecall/cuican-ai-pendant-lights-up-y" target="_blank" title="Movecall CuiCan ESP32S3">ИИ-кулон CuiCan</a>
- <a href="https://github.com/WMnologo/xingzhi-ai" target="_blank" title="WMnologo Xingzhi (безымянная технология, «Звёздный разум») — 1.54">WMnologo Xingzhi-1.54TFT</a>
- <a href="https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html" target="_blank" title="SenseCAP Watcher">SenseCAP Watcher</a>
- <a href="https://www.bilibili.com/video/BV1BHJtz6E2S/" target="_blank" title="Сверхбюджетный робот-пёс ESP-HI">Сверхбюджетный робот-пёс ESP-HI</a>

<div style="display: flex; justify-content: space-between;">
  <a href="docs/v1/lichuang-s3.jpg" target="_blank" title="Плата разработки LiChuang ESP32-S3">
    <img src="docs/v1/lichuang-s3.jpg" width="240" />
  </a>
  <a href="docs/v1/espbox3.jpg" target="_blank" title="Espressif ESP32-S3-BOX3">
    <img src="docs/v1/espbox3.jpg" width="240" />
  </a>
  <a href="docs/v1/m5cores3.jpg" target="_blank" title="M5Stack CoreS3">
    <img src="docs/v1/m5cores3.jpg" width="240" />
  </a>
  <a href="docs/v1/atoms3r.jpg" target="_blank" title="AtomS3R + Echo Base">
    <img src="docs/v1/atoms3r.jpg" width="240" />
  </a>
  <a href="docs/v1/magiclick.jpg" target="_blank" title="Волшебная кнопка 2.4">
    <img src="docs/v1/magiclick.jpg" width="240" />
  </a>
  <a href="docs/v1/waveshare.jpg" target="_blank" title="Waveshare ESP32-S3-Touch-AMOLED-1.8">
    <img src="docs/v1/waveshare.jpg" width="240" />
  </a>
  <a href="docs/v1/lilygo-t-circle-s3.jpg" target="_blank" title="LILYGO T-Circle-S3">
    <img src="docs/v1/lilygo-t-circle-s3.jpg" width="240" />
  </a>
  <a href="docs/v1/xmini-c3.jpg" target="_blank" title="Mini C3 от сяо Гэ">
    <img src="docs/v1/xmini-c3.jpg" width="240" />
  </a>
  <a href="docs/v1/movecall-cuican-esp32s3.jpg" target="_blank" title="CuiCan">
    <img src="docs/v1/movecall-cuican-esp32s3.jpg" width="240" />
  </a>
  <a href="docs/v1/wmnologo_xingzhi_1.54.jpg" target="_blank" title="WMnologo Xingzhi-1.54">
    <img src="docs/v1/wmnologo_xingzhi_1.54.jpg" width="240" />
  </a>
  <a href="docs/v1/sensecap_watcher.jpg" target="_blank" title="SenseCAP Watcher">
    <img src="docs/v1/sensecap_watcher.jpg" width="240" />
  </a>
  <a href="docs/v1/esp-hi.jpg" target="_blank" title="Сверхбюджетный робот-пёс ESP-HI">
    <img src="docs/v1/esp-hi.jpg" width="240" />
  </a>
</div>

## Программное обеспечение

### Прошивка firmware

Новичкам рекомендуется сначала использовать прошивку, которую можно записать без развёртывания среды разработки.

По умолчанию прошивка подключается к официальному серверу [xiaozhi.me](https://xiaozhi.me). Частные пользователи, зарегистрировав аккаунт, могут бесплатно использовать realtime-модель Qwen.

👉 [Руководство по прошивке для новичков](https://ccnphfhqs21z.feishu.cn/wiki/Zpz4wXBtdimBrLk25WdcXzxcnNS)

### Среда разработки

- Cursor или VSCode
- Установите плагин ESP-IDF; рекомендуется [ESP-IDF v6.0.2](https://github.com/espressif/esp-idf/releases/tag/v6.0.2). Предпочтение отдаётся стабильным выпускам v6.0 и новее; ESP-IDF v5.5.2 используется только для сохранения совместимости со старым оборудованием
- Linux предпочтительнее Windows: компиляция быстрее и меньше проблем с драйверами
- В проекте принят стиль кода C++ от Google; при отправке кода убедитесь в его соблюдении

### Документация для разработчиков

- [Руководство по миграции на ESP-IDF 6.0](docs/esp-idf-6-migration.md) — совместимость SDK, изменения компонентов, поддержка старого оборудования и статус проверки плат
- [Руководство по пользовательской плате](docs/custom-board.md) — как создать собственную плату для Сяо Чжи AI
- [Использование MCP для управления IoT](docs/mcp-usage.md) — как управлять IoT-устройствами по протоколу MCP
- [Обмен данными по протоколу MCP](docs/mcp-protocol.md) — реализация протокола MCP на стороне устройства
- [Документ по гибридному протоколу связи MQTT + UDP](docs/mqtt-udp.md)
- [Подробный документ по протоколу связи WebSocket](docs/websocket.md)

## Настройка больших моделей

Если у вас уже есть устройство AI-чат-бота Сяо Чжи, подключённое к официальному серверу, вы можете выполнить настройку в консоли [xiaozhi.me](https://xiaozhi.me).

👉 [Видеоруководство по работе с бэкендом (старый интерфейс)](https://www.bilibili.com/video/BV1jUCUY2EKM/)

## Связанные open-source проекты

Для развёртывания сервера на личном компьютере обратитесь к следующим проектам с открытым исходным кодом:

- [xinnan-tech/xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server) сервер на Python
- [joey-zhou/xiaozhi-esp32-server-java](https://github.com/joey-zhou/xiaozhi-esp32-server-java) сервер на Java
- [AnimeAIChat/xiaozhi-server-go](https://github.com/AnimeAIChat/xiaozhi-server-go) сервер на Golang
- [hackers365/xiaozhi-esp32-server-golang](https://github.com/hackers365/xiaozhi-esp32-server-golang) сервер на Golang

Другие клиентские проекты, использующие протокол связи Сяо Чжи:

- [huangjunsen0406/py-xiaozhi](https://github.com/huangjunsen0406/py-xiaozhi) клиент на Python
- [TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) клиент на Android
- [100askTeam/xiaozhi-linux](http://github.com/100askTeam/xiaozhi-linux) клиент для Linux от «Бай Вэнь Кэцзи» (100ask)
- [78/xiaozhi-sf32](https://github.com/78/xiaozhi-sf32) прошивка Bluetooth-чипа от SiChuan (思澈科技)
- [QuecPython/solution-xiaozhiAI](https://github.com/QuecPython/solution-xiaozhiAI) прошивка QuecPython от Quectel (移遠)

## О проекте

Это ESP32-проект с открытым исходным кодом, опубликованный под лицензией MIT: любой может использовать его бесплатно, в том числе в коммерческих целях.

Мы надеемся, что благодаря этому проекту все смогут разобраться в разработке ИИ-оборудования и применить быстро развивающиеся большие языковые модели к реальным устройствам.

Если у вас есть замечания или предложения, пожалуйста, создавайте Issue или присоединяйтесь к нашему [Discord](https://discord.gg/C759fGMBcZ) либо группе QQ: 1011329060.

## История звёзд

<a href="https://star-history.com/#78/xiaozhi-esp32&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date" />
   <img alt="График истории звёзд" src="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date" />
 </picture>
</a>
