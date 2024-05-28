# Console-Quest
### Скриншот игры

![screenshot](https://github.com/OtryvnoyKalendar/Console-Quest/blob/main/screenshots/screenshot%201.png)

### Общее описание
Это прототип консольной игры, которую я написал для практики языка C. Основной целью являлась практика с чтением и записью файлов, реализация сохранений. Игра сделана на основе видео с ютуб канала Begin End и адаптирована под Linux. Вот *ссылки на видео*:  
[Язык Си с нуля - Урок 39 - Пишем игру квест.](https://www.youtube.com/watch?v=jucHXihq1Xs&list=PLBOPkQsFLCR2DWRY74L03FmbRtz_Yy73_&index=40),  
[Язык Си с нуля - Урок 43 - Пишем игру квест. Добавляем NPC, предметы и задания.](https://www.youtube.com/watch?v=9YGPJM1cWC8&list=PLBOPkQsFLCR2DWRY74L03FmbRtz_Yy73_&index=44).

### Как играть
Чтобы начать играть, нужно открыть консоль и перейти в папку с проектом. Дальше просто выполните скрипт:
```sh
sh project_start
```

- Управление:  
'Esc' - выйти из игры  
'A' - влево  
'D' - вправо  
'W' - вверх  
'S' - вниз  

Если вы хотите **изменить код игры**, то вам не нужно ничего перекомпелировать и удалять вручную. После внесения изменений в файлы игры, просто снова запустите скрипт `project_start`, он всё сделает за вас.

Если вы захотите воспользоваться **отладчиком GDB**, чтобы дебажить код, то скрипт нужно запускать так:
```sh
sh project_start gdb
```
Но из-за специфики работы игры, отладить ничего всё равно не получиться.

### Технические подробности
В процессе написания игры я сделал свои хидеры, которые можно использовать для других проектов. Заголовочный файл `core.h` нужен для включения в консоли `режима ncurses`.

Я добился следующих важных моментов:
1. в игре нет миганий
2. есть возможность создавать объекты разных цветов
3. Можно сохраняться под разными пользователями, если изменить код загрузчика файлов
