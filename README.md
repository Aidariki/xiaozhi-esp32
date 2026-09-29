# Чат-бот на основе MCP

([Русский](README.md) | [English](https://github.com/78/xiaozhi-esp32/blob/main/README.md) | [中文](README_zh.md) | [日本語](README_ja.md))

## Введение

👉 [Человек: даёт ИИ камеру, а ИИ: сразу вычислил, что хозяин не мыл голову три дня【bilibili】](https://www.bilibili.com/video/BV1bpjgzKEhd/)

👉 [Создай свою ИИ-девушку своими руками: руководство для новичков【bilibili】](https://www.bilibili.com/video/BV1XnmFYLEJN/)

AI-чат-бот XiaoZhi («Сяо Чжи») служит точкой голосового взаимодействия: он использует возможности больших моделей, таких как Qwen / DeepSeek, и обеспечивает управление множеством устройств через протокол MCP.

<img src="docs/mcp-based-graph.jpg" alt="Управление всем через MCP" width="320">

## Недавние обновления

- Основная ветвь проекта теперь ориентирована на ESP-IDF v6.0 или новее; предпочтительная стабильная версия SDK — v6.0.2. Прежняя база из 157 вариантов была проверена на ESP-IDF v6.0.1; текущая матрица содержит 171 вариант, из которых 170 поддерживают IDF 6.0.x, а вариант для ESP32-S31 требует IDF 6.1 или новее.
- Криптографический код MQTT и BluFi перенесён на PSA Crypto. Также устранены вопросы совместимости с разделением компонентов в IDF 6 и со сторонними зависимостями.
- Усовершенствованы параллельная обработка аудиопотока, проверка пакетов MQTT/UDP и выбор релизной матрицы.
- ESP-IDF v5.5 сохранён только для задокументированных старых плат. ESP32-P4 Rev1 и Rev3 оба поддерживаются на IDF 6 с ESP-SR 2.4.7; подробности о совместимости и статусе проверки плат см. в [Руководстве по миграции на ESP-IDF 6.0](docs/esp-idf-6-migration.md).

### Реализованные возможности

- Wi-Fi, проводной Ethernet, USB RNDIS, а также сети 4G Cat.1 на модулях ML307/EC801E или NT26; поддерживаемые платы могут переключаться между Wi-Fi и 4G
- Офлайн-активация голосом на базе [ESP-SR](https://github.com/espressif/esp-sr), включая настраиваемые слова активации
- Два транспортных протокола связи: [WebSocket](docs/websocket.md) и [MQTT + UDP](docs/mqtt-udp.md)
- Аудиовещание в Opus с традиционными конвейерами потоковых ASR + LLM + TTS, а также с end-to-end realtime-голосовыми моделями; оборудование с поддержкой AEC позволяет вести realtime-диалог в полном дуплексе
- Распознавание голоса: определяет текущего говорящего [3D Speaker](https://github.com/modelscope/3D-Speaker)
- OLED / LCD-дисплеи с поддержкой эмодзи и богатой эмоциональной выразительности, а на поддерживаемых платах — ещё и визуальный ввод с камеры
- Отображение заряда батареи и управление электропитанием
- 39 языков интерфейса, локализованные голосовые подсказки там, где они доступны, с откатом на английский
- Платформы чипов ESP32, ESP32-C3, ESP32-C5, ESP32-C6, ESP32-S3 и ESP32-P4
- Настройка Wi-Fi через точку доступа или BluFi
- Пристроенный MCP (на стороне устройства) для управления им (динамик, светодиоды, сервоприводы, GPIO и т. д.)
- Облачный MCP для расширения возможностей большой модели (управление умным домом, работа с рабочим столом ПК, поиск знаний, электронная почта и т. д.)
- Настраиваемые слова активации, шрифты, эмодзи и фоны чата с онлайн-редактированием через веб-интерфейс ([Генератор пользовательских ресурсов](https://github.com/78/xiaozhi-assets-generator))

## Оборудование

### Практика сборки на беспаечной макетной плате

См. руководство в документе Feishu:

👉 [«Энциклопедия AI-чат-бота XiaoZhi»](https://ccnphfhqs21z.feishu.cn/wiki/F5krwD16viZoF0kKkvDcrZNYnhb?from=from_copylink)

Демонстрация на макетной плате:

![Демонстрация на макетной плате](docs/v1/wiring2.jpg)

### Поддержка 138 каталогов плат и 171 релизного варианта (частичный список)

- <a href="https://oshwhub.com/li-chuang-kai-fa-ban/li-chuang-shi-zhan-pai-esp32-s3-kai-fa-ban" target="_blank" title="Плата разработки LiChuang ESP32-S3">Плата разработки LiChuang ESP32-S3</a>
- <a href="https://github.com/espressif/esp-box" target="_blank" title="Espressif ESP32-S3-BOX-3">Espressif ESP32-S3-BOX-3</a>
- <a href="https://docs.m5stack.com/zh_CN/core/CoreS3" target="_blank" title="M5Stack CoreS3">M5Stack CoreS3</a>
- <a href="https://docs.m5stack.com/en/atom/Atomic%20Echo%20Base" target="_blank" title="AtomS3R + Echo Base">M5Stack AtomS3R + Echo Base</a>
- <a href="https://gf.bilibili.com/item/detail/1108782064" target="_blank" title="Волшебная кнопка 2.4">Волшебная кнопка 2.4</a>
- <a href="https://www.waveshare.net/shop/ESP32-S3-Touch-AMOLED-1.8.htm" target="_blank" title="Waveshare ESP32-S3-Touch-AMOLED-1.8">Waveshare ESP32-S3-Touch-AMOLED-1.8</a>
- <a href="https://github.com/Xinyuan-LilyGO/T-Circle-S3" target="_blank" title="LILYGO T-Circle-S3">LILYGO T-Circle-S3</a>
- <a href="https://oshwhub.com/tenclass01/xmini_c3" target="_blank" title="XiaGe Mini C3">XiaGe Mini C3</a>
- <a href="https://oshwhub.com/movecall/cuican-ai-pendant-lights-up-y" target="_blank" title="Movecall CuiCan ESP32S3">ИИ-кулон CuiCan</a>
- <a href="https://github.com/WMnologo/xingzhi-ai" target="_blank" title="WMnologo-Xingzhi-1.54">WMnologo-Xingzhi-1.54TFT</a>
- <a href="https://www.seeedstudio.com/SenseCAP-Watcher-W1-A-p-5979.html" target="_blank" title="SenseCAP Watcher">SenseCAP Watcher</a>
- <a href="https://www.bilibili.com/video/BV1BHJtz6E2S/" target="_blank" title="Низкобюджетный робот-пёс ESP-HI">Низкобюджетный робот-пёс ESP-HI</a>

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
  <a href="docs/v1/xmini-c3.jpg" target="_blank" title="XiaGe Mini C3">
    <img src="docs/v1/xmini-c3.jpg" width="240" />
  </a>
  <a href="docs/v1/movecall-cuican-esp32s3.jpg" target="_blank" title="CuiCan">
    <img src="docs/v1/movecall-cuican-esp32s3.jpg" width="240" />
  </a>
  <a href="docs/v1/wmnologo_xingzhi_1.54.jpg" target="_blank" title="WMnologo-Xingzhi-1.54">
    <img src="docs/v1/wmnologo_xingzhi_1.54.jpg" width="240" />
  </a>
  <a href="docs/v1/sensecap_watcher.jpg" target="_blank" title="SenseCAP Watcher">
    <img src="docs/v1/sensecap_watcher.jpg" width="240" />
  </a>
  <a href="docs/v1/esp-hi.jpg" target="_blank" title="Низкобюджетный робот-пёс ESP-HI">
    <img src="docs/v1/esp-hi.jpg" width="240" />
  </a>
</div>

## Программное обеспечение

### Прошивка устройства

Новичкам рекомендуется использовать прошивку, которую можно прошить без развёртывания среды разработки.

По умолчанию такая прошивка подключается к официальному серверу [xiaozhi.me](https://xiaozhi.me). Частные пользователи могут зарегистрировать аккаунт, чтобы бесплатно использовать realtime-модель Qwen.

👉 [Руководство по прошивке для новичков](https://ccnphfhqs21z.feishu.cn/wiki/Zpz4wXBtdimBrLk25WdcXzxcnNS)

### Среда разработки

- Cursor или VSCode
- Установите плагин ESP-IDF. Предпочтительна [ESP-IDF v6.0.2](https://github.com/espressif/esp-idf/releases/tag/v6.0.2); используйте стабильный выпуск v6.0 или новее. ESP-IDF v5.5.2 сохранён только для обратной совместимости со старыми платами
- Linux лучше, чем Windows: компиляция быстрее и меньше проблем с драйверами
- В этом проекте используется стиль кода C++ от Google; пожалуйста, соблюдайте его при отправке кода

### Документация для разработчиков

- [Руководство по миграции на ESP-IDF 6.0](docs/esp-idf-6-migration.md) — совместимость SDK, изменения компонентов, поддержка старого оборудования и статус проверки плат
- [Руководство по созданию собственной платы](docs/custom-board.md) — узнайте, как создавать пользовательские платы для XiaoZhi AI
- [Использование MCP для управления IoT-устройствами](docs/mcp-usage.md) — как управлять IoT-устройствами по протоколу MCP
- [Взаимодействие по протоколу MCP](docs/mcp-protocol.md) — реализация протокола MCP на стороне устройства
- [Документ по гибридному протоколу связи MQTT + UDP](docs/mqtt-udp.md)
- [Подробный документ по протоколу связи WebSocket](docs/websocket.md)

## Конфигурация больших моделей

Если у вас уже есть устройство AI-чат-бота XiaoZhi, подключённое к официальному серверу, вы можете войти в консоль [xiaozhi.me](https://xiaozhi.me) для настройки.

👉 [Видеоруководство по работе с бэкендом (старый интерфейс)](https://www.bilibili.com/video/BV1jUCUY2EKM/)

## Связанные проекты с открытым исходным кодом

Для развёртывания сервера на личном компьютере обратитесь к следующим проектам с открытым исходным кодом:

- [xinnan-tech/xiaozhi-esp32-server](https://github.com/xinnan-tech/xiaozhi-esp32-server) сервер на Python
- [joey-zhou/xiaozhi-esp32-server-java](https://github.com/joey-zhou/xiaozhi-esp32-server-java) сервер на Java
- [AnimeAIChat/xiaozhi-server-go](https://github.com/AnimeAIChat/xiaozhi-server-go) сервер на Golang
- [hackers365/xiaozhi-esp32-server-golang](https://github.com/hackers365/xiaozhi-esp32-server-golang) сервер на Golang

Другие клиентские проекты, использующие протокол связи XiaoZhi:

- [huangjunsen0406/py-xiaozhi](https://github.com/huangjunsen0406/py-xiaozhi) клиент на Python
- [TOM88812/xiaozhi-android-client](https://github.com/TOM88812/xiaozhi-android-client) клиент на Android
- [100askTeam/xiaozhi-linux](http://github.com/100askTeam/xiaozhi-linux) клиент для Linux от 100ask
- [78/xiaozhi-sf32](https://github.com/78/xiaozhi-sf32) прошивка Bluetooth-чипа от Sichuan
- [QuecPython/solution-xiaozhiAI](https://github.com/QuecPython/solution-xiaozhiAI) прошивка QuecPython от Quectel

Инструменты пользовательских ресурсов:

- [78/xiaozhi-assets-generator](https://github.com/78/xiaozhi-assets-generator) Генератор пользовательских ресурсов (слова активации, шрифты, эмодзи, фоны)

## О проекте

Это проект с открытым исходным кодом для ESP32, распространяемый по лицензии MIT, которая позволяет любому использовать его бесплатно, в том числе в коммерческих целях.

Мы надеемся, что этот проект поможет всем понять разработку ИИ-оборудования и применить быстро развивающиеся большие языковые модели к реальным аппаратным устройствам.

Если у вас есть идеи или предложения, смело создавайте Issue или присоединяйтесь к нашему [Discord](https://discord.gg/C759fGMBcZ) или группе QQ: 1095994019

## История звёзд

<a href="https://star-history.com/#78/xiaozhi-esp32&Date">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date" />
   <img alt="График истории звёзд" src="https://api.star-history.com/svg?repos=78/xiaozhi-esp32&type=Date" />
 </picture>
</a>
